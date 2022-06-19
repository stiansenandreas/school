#include "mbed.h"

#include "DFRobot_RGBLCD.h"
#include "HTS221Sensor.h"
#include "DevI2C.h"
#include "json.hpp"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>


using json = nlohmann::json;

enum buffers { 
    HTTP_REQUEST_BUFFER_SIZE = 200,
    HTTP_RESPONSE_BUFFER_SIZE = 6000,
    LCD_SCREEN_WIDTH = 16
};


// network api paths and hosts
constexpr char epoch_time_api_path[] = "/UnixTime/tounixtimestamp?datetime=now";
constexpr char epoch_time_api_host[] = "showcase.api.linx.twenty57.net";

constexpr char weather_api_path[] =  "/api/current?lat=58.3&lon=8.6"; // Grimstad coordinates
constexpr char weather_api_host[] =  "fcc-weather-api.glitch.me";

constexpr char news_feed_api_path[] = "/TheHackersNews?format=xml";
constexpr char news_feed_api_host[] = "feeds.feedburner.com";

constexpr char json_token[] = "{";
constexpr char xml_token[] = "<";



// internal module and I/O objects
DevI2C i2c(PB_11, PB_10);
HTS221Sensor hts221(&i2c);
DFRobot_RGBLCD lcd(LCD_SCREEN_WIDTH, 2, PB_9, PB_8);
EventQueue* queue = mbed_event_queue();

InterruptIn button1(PD_14);
InterruptIn button2(PB_0);
InterruptIn button3(PA_3);
PwmOut buzzer(PB_4);

// global objects and flags
Timeout alarm;
NetworkInterface* network;
Mutex socket_mutex;

time_t alarm_seconds = 0;
bool alarm_set = false;
bool alarm_mute = false;
bool alarm_not_set = true;


class screen {
    // function pointers for button functions
    void (*button_1)();
    void (*button_2)();
    void (*button_3)();

    void (*button_2_2)();
    void (*button_3_2)();

    char* line_1{0};
    char* line_2{0};

    screen* next_screen;
    Mutex mutex;

    public:
   
    static screen* current_screen;

    screen(void (*func_1)(), void (*func_2)(), void (*func_3)()) {
        button_1 = func_1;
        button_2 = func_2;
        button_3 = func_3;
    }

    screen(void (*func_1)(), void (*func_2)(), void (*func_3)(), void (*func_2_2)(), void (*func_3_2)()) {
        button_1 = func_1;
        button_2 = func_2;
        button_3 = func_3;
        button_2_2 = func_2_2;
        button_3_2 = func_3_2;
    }

    void init_buttons() {
        mutex.lock(); // attach functions to buttons and put them in event queue
        button1.fall(queue->event(button_1));
        button2.fall(queue->event(button_2));
        button3.fall(queue->event(button_3));
        mutex.unlock();
    }

    void switch_button_2() { button2.fall(queue->event(button_2_2)); }
    void reset_button_2() { button2.fall(queue->event(button_2)); }

    void switch_button_3() { button3.fall(queue->event(button_3_2)); }
    void reset_button_3() { button3.fall(queue->event(button_3)); }

    char* get_line_1() { return line_1; }
    char* get_line_2() { return line_2; }

    void set_next_screen(screen* next) {
        next_screen = next;
    }

    void cycle_screen() { 
        mutex.lock();
        current_screen = next_screen;
        mutex.unlock();
        }


    void set_line_1(char* input) {
        mutex.lock();
        line_1 = input;
        mutex.unlock();
    }

    void set_line_2(char* input) {
        mutex.lock();
        line_2 = input;
        mutex.unlock();
    }


    static void print_to_screen(screen* screen) {
        int i = 0;
        int len = strlen(screen->line_1);
    
        // scroll if line 1 is longer than screen width
        do {
            screen->mutex.lock();
            
            // break if screen changes
            if (screen != current_screen) {
                screen->mutex.unlock();
                break;
            }

            lcd.clear();
            lcd.printf("%.16s", &screen->line_1[i]);
            
            lcd.setCursor(0, 1);
            lcd.printf(screen->line_2);
            
            screen->mutex.unlock();
            i++;
            
            if (i > 1)
                thread_sleep_for(250);
    
    
        }while (i + LCD_SCREEN_WIDTH <= len);
    }
}; screen* screen::current_screen;


// button functions
void screen_switch() {
    screen::current_screen->cycle_screen();
    screen::current_screen->init_buttons();
    screen::current_screen->print_to_screen(screen::current_screen);
}


void sound_alarm() {
    if (!alarm_mute) { // sound buzzer
        buzzer.period_us(5 * 63);
        buzzer = 0.5f;
    }
    
    // reset flags and alarm time
    alarm_seconds = 0;
    alarm_set = false;
    alarm_mute = false;
}


void inc_alarm() {
    alarm_seconds += 60; // increase by 1 minute
    char current_alarm[LCD_SCREEN_WIDTH + 1] = {0};
    strftime(current_alarm, LCD_SCREEN_WIDTH + 1, "%R", localtime(&alarm_seconds));
    
    screen::current_screen->set_line_1("Set alarm");
    screen::current_screen->set_line_2(current_alarm);
    screen::current_screen->print_to_screen(screen::current_screen);
}


void confirm_alarm() {
    // difference between set alarm time and now
    auto alarm_time = std::chrono::seconds(alarm_seconds - time(NULL));
    alarm.attach(&sound_alarm, alarm_time);
    
    // change button functionality back
    screen::current_screen->reset_button_2();
    screen::current_screen->reset_button_3();
    screen::current_screen->set_line_1("Alarm enabled");
    
    char current_alarm[LCD_SCREEN_WIDTH + 1] = {0};
    strftime(current_alarm, LCD_SCREEN_WIDTH + 1, "%R", localtime(&alarm_seconds));
    
    screen::current_screen->set_line_2(current_alarm);
    screen::current_screen->print_to_screen(screen::current_screen);
    
    alarm_not_set = false;
    alarm_set = true;
}


void set_alarm() {
    alarm_seconds = time(NULL);
    alarm_seconds = alarm_seconds + 60 - 1 - (alarm_seconds + 60 - 1) % 60; // round up to nearest 1 minute mark
    
    char current_alarm[LCD_SCREEN_WIDTH + 1] = {0};
    strftime(current_alarm, LCD_SCREEN_WIDTH + 1, "%R", localtime(&alarm_seconds));
    
    screen::current_screen->set_line_1("Set alarm");
    screen::current_screen->set_line_2(current_alarm);

    // change functionality on button 2 and 3
    screen::current_screen->switch_button_2();
    screen::current_screen->switch_button_3();
    
    screen::current_screen->print_to_screen(screen::current_screen);
}


void toggle_alarm() {
    if (alarm_set) { // turn off if set
        alarm.detach();
        alarm_set = false;
        
        char current_alarm[LCD_SCREEN_WIDTH + 1] = {0};
        strftime(current_alarm, LCD_SCREEN_WIDTH + 1, "%R", localtime(&alarm_seconds));

        screen::current_screen->set_line_1("Alarm disabled");
        screen::current_screen->set_line_2(current_alarm);
        screen::current_screen->print_to_screen(screen::current_screen);
    }
    else if (!alarm_set and !alarm_not_set) { // turn on if not set but dont turn on if an alarm was never set
        auto alarm_time = std::chrono::seconds(alarm_seconds - time(NULL));
        alarm.attach(&sound_alarm, alarm_time);
        alarm_set = true;
        
        char current_alarm[LCD_SCREEN_WIDTH + 1] = {0};
        strftime(current_alarm, LCD_SCREEN_WIDTH + 1, "%R", localtime(&alarm_seconds));
        
        screen::current_screen->set_line_1("Alarm enabled");
        screen::current_screen->set_line_2(current_alarm);
        screen::current_screen->print_to_screen(screen::current_screen);
    }
}


void mute_alarm() {
    buzzer = 0; // turn off buzzer

    char current_alarm[LCD_SCREEN_WIDTH + 1] = {0};

    if (alarm_set) {
         if (!alarm_mute) {
            strftime(current_alarm, LCD_SCREEN_WIDTH + 1, "Alarm MUTE %R", localtime(&alarm_seconds));
            alarm_mute = true;
         } else {
             alarm_mute = false;
             strftime(current_alarm, LCD_SCREEN_WIDTH + 1, "Alarm ON %R", localtime(&alarm_seconds));
         }
    } else {
        alarm_mute = false;
        strftime(current_alarm, LCD_SCREEN_WIDTH + 1, "Alarm OFF %R", localtime(&alarm_seconds));
    }

    screen::current_screen->set_line_2(current_alarm);
    screen::current_screen->print_to_screen(screen::current_screen);
}


void snooze_alarm() {
    if (alarm_set) {
        alarm_seconds += 300;
        alarm.detach();

        auto alarm_time = std::chrono::seconds(alarm_seconds - time(NULL));
        alarm.attach(&sound_alarm, alarm_time);
    
        char current_alarm[LCD_SCREEN_WIDTH + 1] = {0};
        
        strftime(current_alarm, LCD_SCREEN_WIDTH + 1, "Alarm ON %R", localtime(&alarm_seconds));

        screen::current_screen->set_line_2(current_alarm);
        screen::current_screen->print_to_screen(screen::current_screen);
    }
}


void no_function() {}



// network functions
void network_connect() {
    network = NetworkInterface::get_default_instance();
    if (!network)
        printf("\nFailed to get default network interface\n");

    printf("\nConnecting to the network...\n");
    
    nsapi_size_or_error_t result = network->connect();
    if (result != 0)
        printf("\nFailed to connect to network: %d\n", result);
}


char* get_request(const char* file_path, const char* host_name, const char* token) {
    
    // connect socket
    // as sockets are expensive we use a mutex to ensure only one socket object exists at a time
    socket_mutex.lock();
    TCPSocket *socket = new TCPSocket;
    
    socket->open(network);
    socket->set_timeout(1000);
    
    SocketAddress address;
    
    nsapi_size_or_error_t result = network->gethostbyname(host_name, &address);
    
    if (result != 0) {
        printf("\nFailed to get IP address of host: %d\n", result);
        socket_mutex.unlock();
        delete socket;
        return nullptr;
    }
    
    address.set_port(80);
    
    result = socket->connect(address);
    
    if (result != 0) {
        printf("\nFailed to connect to server: %d\n", result);
        socket_mutex.unlock();
        delete socket;
        return nullptr;
    }


    // send request
    char* request = (char*) malloc(sizeof(char) * HTTP_REQUEST_BUFFER_SIZE);
    
    snprintf(request, HTTP_REQUEST_BUFFER_SIZE,
                "GET %s HTTP/1.1\r\n"
  			    "Host: %s\r\n"
				"Connection: close\r\n"
                "User-Agent: Microcontroller\r\n"
                "\r\n", file_path, host_name);
    
    nsapi_size_t bytes_to_send = strlen(request);
    nsapi_size_or_error_t bytes_sent = 0;

    while (bytes_to_send) {
        bytes_sent = socket->send(request + bytes_sent, bytes_to_send);

        if (bytes_sent < 0) {
            printf("\nError: %d\n", bytes_sent);
            socket_mutex.unlock();
            delete socket;
            free(request);
            return nullptr;
        } 
        else
            printf("\nSent %d bytes\n", bytes_sent);

        bytes_to_send -= bytes_sent;
    }

    free(request);

    // recieve response
    char* response = (char*) malloc(sizeof(char) * HTTP_RESPONSE_BUFFER_SIZE);

    int remaining_bytes = HTTP_RESPONSE_BUFFER_SIZE;
    int recieved_bytes = 0;
    
    while (remaining_bytes > 0) {
        nsapi_size_or_error_t result = socket->recv(response + recieved_bytes, remaining_bytes);
        
        if(result == 0) {
            response[recieved_bytes] = '\0';
            break;
        }

        if(result < 0) {
            printf("\nError: %d\n", result);
            socket_mutex.unlock();
            delete socket;
            free(response);
            return nullptr;
        }

        printf("\nRecieved %d bytes\n", result);
        
        recieved_bytes += result;
        remaining_bytes -= result;
    }

    socket->close();
    delete socket;
    socket_mutex.unlock();

    // extract response body
    char* body = strchr(response, *token);
    free(response);
    return body;
}



// background functions
void update_clock(screen* screen) {
    char default_line_2[] = "Alarm OFF 00:00";
    char clock_date[LCD_SCREEN_WIDTH + 1] = {0};
    
    time_t seconds = time(NULL); // get epoch time seconds from rtc
    strftime(clock_date, LCD_SCREEN_WIDTH + 1, "%a %d %b %R", localtime(&seconds));
    
    screen->set_line_1(clock_date);
    screen->set_line_2(default_line_2);
    screen->print_to_screen(screen); // print once on thread startup, thereafter only when needed

    while(true) {
        time_t seconds = time(NULL);
        char current_alarm[LCD_SCREEN_WIDTH + 1] = {0};
        
        if (alarm_set) // update alarm status
            strftime(current_alarm, LCD_SCREEN_WIDTH + 1, "Alarm ON %R", localtime(&alarm_seconds));
        else
            strftime(current_alarm, LCD_SCREEN_WIDTH + 1, "Alarm OFF %R", localtime(&alarm_seconds));
        
        if (screen->get_line_2() != current_alarm) {
            screen->set_line_2(current_alarm);
            screen->print_to_screen(screen);
        }

        // update clock and date
        char clock_date[LCD_SCREEN_WIDTH + 1] = {0};
        strftime(clock_date, LCD_SCREEN_WIDTH + 1, "%a %d %b %R", localtime(&seconds));
        screen->set_line_1(clock_date);
        
        // only update screen if shown and only on whole minutes
        if (screen == screen::current_screen and seconds % 60 == 0) {
            screen->print_to_screen(screen);
        }
        thread_sleep_for(1000);
    }
}


void update_temp_hum(screen* screen) {
    while (true) {
        // get values from sensor and put into string
        char temp_str[LCD_SCREEN_WIDTH + 1];
        float temp;
        hts221.get_temperature(&temp);
        snprintf(temp_str, LCD_SCREEN_WIDTH + 1, "Temp: %.1f C", temp);
        
        char hum_str[LCD_SCREEN_WIDTH + 1];
        float hum;
        hts221.get_humidity(&hum);
        snprintf(hum_str, LCD_SCREEN_WIDTH + 1, "Hum:  %.1f %s", hum, "%%");

        // put into screen object
        screen->set_line_1(temp_str);
        screen->set_line_2(hum_str);
        
        // update screen if shown
        if (screen == screen::current_screen)
            screen->print_to_screen(screen);

        thread_sleep_for(10000);
    }
}


void update_weather(screen* screen) {
    char weather_default[] = "Loading weather";
    screen->set_line_1(weather_default);
    
    while (true) {
        printf("\nFetching weather\n");
        char* json_start = nullptr;
        json_start = get_request(weather_api_path, weather_api_host, json_token);

        if (json_start == nullptr) { // try again
            thread_sleep_for(5000);
            continue;
        }

        json weather_json = json::parse(json_start, nullptr, false);
        if (weather_json.is_discarded()) {
            thread_sleep_for(5000);
            continue;
        }

        // extract weather variables
        std::string weather_description = weather_json["weather"][0]["main"].get<std::string>();
        float weather_temperature = weather_json["main"]["temp"].get<float>();
        float weather_wind_speed = weather_json["wind"]["speed"].get<float>();
        int weather_clouds_percentage = weather_json["clouds"]["all"].get<int>();
    
        // put into strings
        char weather_line_1[LCD_SCREEN_WIDTH + 1];
        snprintf(weather_line_1, LCD_SCREEN_WIDTH + 1, "%s  %.1f C", weather_description.c_str(), weather_temperature);

        char weather_line_2[LCD_SCREEN_WIDTH + 1];
        snprintf(weather_line_2, LCD_SCREEN_WIDTH + 1, "Cover %i%s %.0fm/s", weather_clouds_percentage,"%%", weather_wind_speed);

        // put into screen object
        screen->set_line_1(weather_line_1);
        screen->set_line_2(weather_line_2);

        // update screen if shown
        if (screen == screen::current_screen)
            screen->print_to_screen(screen);

        break;
    }
}


void update_news(screen* screen) {
    char news_default[] = "Fetching news";
    screen->set_line_1(news_default);

    char* xml_start = (char*) malloc(sizeof(char) * HTTP_RESPONSE_BUFFER_SIZE);
    while (true) {
        printf("\nFetching news\n");
        xml_start = get_request(news_feed_api_path, news_feed_api_host, xml_token);

        if (xml_start == nullptr) { // try again
            thread_sleep_for(5000);
            continue;
        }
        
        char titles[5][250] = {0};
        char* next_title = strstr(xml_start, "<title>"); // first title

        // find 4 more titles from xml, in this xml the first two title tags are not news headlines
        for (int i = 0; i < 5; i++) {
            next_title = strstr(next_title, "<title>") + 7; // go past start tag
            
            char* title_end = strstr(next_title, "</title>");
            int diff = (int)(title_end - next_title);
            
            memcpy(titles[i], next_title, diff);

            next_title = title_end + 8; // go past end tag
            titles[i][diff] = '\0'; 
        }

        free(xml_start);

        char line_1[750] = {0}; // store as one long string for printing to screen
        sprintf(line_1, "%s --- %s --- %s", titles[2], titles[3], titles[4]);
        
        screen->set_line_1(line_1);
        screen->set_line_2(titles[0]); // show feed name on second line
        
        // update screen if shown
        if (screen == screen::current_screen)
                screen->print_to_screen(screen);
        
        break;
    }
}



int main() {
    // instancing the different screens with their respective button functions 1-3 or more
    screen* default_screen = new screen(screen_switch, snooze_alarm, mute_alarm);
    screen* alarm_screen = new screen(screen_switch, set_alarm, toggle_alarm, confirm_alarm, inc_alarm);
    screen* temp_hum_screen = new screen(screen_switch, no_function, no_function);
    screen* weather_screen = new screen(screen_switch, no_function, no_function);
    screen* news_feed_screen = new screen(screen_switch, no_function, no_function);


    // linked loop for cycling through the screens
    default_screen->set_next_screen(alarm_screen);
    alarm_screen->set_next_screen(temp_hum_screen);
    temp_hum_screen->set_next_screen(weather_screen);
    weather_screen->set_next_screen(news_feed_screen);
    news_feed_screen->set_next_screen(default_screen);


     // init network connection
    Thread network_init;
    network_init.start(callback(network_connect));


    // init modules
    lcd.init();

    if (hts221.init(NULL) != 0) 
        printf("Failed to initialize\n");

    if (hts221.enable() != 0) 
        printf("Failed to power up\n");


    // retrieve epoch time and set rtc
    network_init.join();
    char* json_start =  nullptr;

    while (json_start == nullptr) {
        printf("\nFetching epoch time\n");
        json_start = get_request(epoch_time_api_path, epoch_time_api_host, json_token);
    
        json epoch_json = json::parse(json_start, nullptr, false);
        if (epoch_json.is_discarded()) {
            thread_sleep_for(500);
            continue; // try again
        }

        std::string epoch_str = epoch_json["UnixTimeStamp"].get<std::string>();
        time_t epoch_time = (time_t) std::stoi(epoch_str) + 2 * 3600; // norwegian summer time is two hours ahead
        set_time(epoch_time);
    }


    // startup
    for (int i = 0; i < 5; i++) {
        lcd.clear();
        lcd.printf("Unix epoch time:");
        
        lcd.setCursor(0, 1);
        lcd.printf("%i", time(NULL));
        
        thread_sleep_for(1000);
    }
    
    // default screen
    screen::current_screen = default_screen;
    screen::current_screen->init_buttons();
    
    char alarm_default_1[] = "No alarm set";
    char alarm_default_2[] = "00:00";

    alarm_screen->set_line_1(alarm_default_1);
    alarm_screen->set_line_2(alarm_default_2);


    // threads
    // clock
    Thread clock_thread(osPriorityRealtime, 1024);
    clock_thread.start(callback(update_clock, default_screen));
    

    // temperature and humidity 
    Thread temp_hum_thread(osPriorityNormal, 1024);
    temp_hum_thread.start(callback(update_temp_hum, temp_hum_screen));


    // weather
    Thread weather_thread(osPriorityNormal, 4096);
    weather_thread.start(callback(update_weather, weather_screen));


    // news
    Thread news_feed_thread(osPriorityNormal, 4096);
    news_feed_thread.start(callback(update_news, news_feed_screen));


    // keep program running
    while (true) {
        // thread states
        printf( "\n--- thread states ---"
                "\nclock: %d"
                "\ntemp_hum: %d"
                "\nweather: %d"
                "\nnews: %d\n",
                clock_thread.get_state(), temp_hum_thread.get_state(),
                weather_thread.get_state(), news_feed_thread.get_state());

        // memory stats code borrowed from https://os.mbed.com/blog/entry/Tracking-memory-usage-with-Mbed-OS/
        // allocate enough room for every thread's stack statistics
        int cnt = osThreadGetCount();
        mbed_stats_stack_t *stats = (mbed_stats_stack_t*) malloc(cnt * sizeof(mbed_stats_stack_t));
 
        cnt = mbed_stats_stack_get_each(stats, cnt);
        for (int i = 0; i < cnt; i++) {
            printf("Thread: 0x%lX, Stack size: %lu / %lu\r\n", stats[i].thread_id, stats[i].max_size, stats[i].reserved_size);
        }
        free(stats);
 
        // Grab the heap statistics
        mbed_stats_heap_t heap_stats;
        mbed_stats_heap_get(&heap_stats);
        printf("Heap size: %lu / %lu bytes\r\n", heap_stats.current_size, heap_stats.reserved_size);
        thread_sleep_for(10000);
    }
      
}

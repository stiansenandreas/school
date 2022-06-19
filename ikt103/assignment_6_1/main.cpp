#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/uri.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace utility;

void menu() {
    std::cout << "1. Read all students\n"
                 "2. Get student by id\n"
                 "3. Add student\n"
                 "4. Edit student\n"
                 "5. Remove student\n"
                 "6. Exit\n";
}

void printStudent(const json::value &student) {
    ucout << "id: " << student.at(U("id"))
          << ", name: " << student.at(U("name")).as_string()
          << ", email: " << student.at(U("email")).as_string()
          << ", year: " << student.at(U("year"))
          << std::endl;
}


int main() {
    http_client client(U("http://localhost:5000"));

    int option = 0;
    while (option != 6) {
        menu();
        std::cin >> option;
        std::cin.ignore();

        switch (option) {
            default: {
                std::cout << "Invalid option" << std::endl;
                break;
            }

            case 1: {
                auto request = client.request(methods::GET, uri_builder(U("students")).to_string())
                        .then([](const http_response &response) {
                            if (response.status_code() != 200)
                                throw std::runtime_error("Request failed: " + std::to_string(response.status_code()));

                            return response.extract_json();
                        })

                        .then([](const json::value &students) {
                            for (auto &student: students.as_array())
                                printStudent(student);
                        });
                break;
            }

            case 2: {
                string_t id;
                ucin >> id;
                std::cin.ignore();

                auto request = client.request(methods::GET, uri_builder(U("students")).append(id).to_string())
                        .then([](const http_response &response) {
                            if (response.status_code() == 404) {
                                std::cout << "Student not found" << std::endl;
                                pplx::cancel_current_task();
                            }

                            if (response.status_code() != 200)
                                throw std::runtime_error("Request failed: " + std::to_string(response.status_code()));

                            return response.extract_json();
                        })

                        .then([](const json::value &student) {
                            printStudent(student);
                        });
                break;
            }

            case 3: {
                string_t name, email;
                int year;

                std::getline(ucin, name);
                ucin >> email;
                std::cin >> year;

                json::value studentJson;

                studentJson[U("name")] = json::value::string(name);
                studentJson[U("email")] = json::value::string(email);
                studentJson[U("year")] = json::value::number(year);

                auto request = client.request(
                                    methods::POST,
                                    uri_builder(U("students/")).to_string(),
                                    studentJson.serialize(),
                                    U("application/json")
                                    )

                        .then([](const http_response &response) {
                            if (response.status_code() != 201)
                                throw std::runtime_error("Request failed: " + std::to_string(response.status_code()));

                            return response.extract_json();
                        })

                        .then([](const json::value &student) {
                            ucout << "Added student: ";
                            printStudent(student);
                        });
                break;
            }

            case 4: {
                string_t id, name, email;
                int year;

                ucin >> id;
                std::cin.ignore();
                std::getline(ucin, name);
                ucin >> email;
                std::cin >> year;

                json::value studentJson;

                studentJson[U("id")] = json::value::number(std::stoi(id));
                studentJson[U("name")] = json::value::string(name);
                studentJson[U("email")] = json::value::string(email);
                studentJson[U("year")] = json::value::number(year);

                auto request = client.request(
                                    methods::PUT,
                                    uri_builder(U("students")).append(id).to_string(),
                                    studentJson.serialize(),
                                    U("application/json")
                                    )

                        .then([](const http_response &response) {
                            if (response.status_code() == 404) {
                                std::cout << "Student not found" << std::endl;
                                pplx::cancel_current_task();
                            }

                            if (response.status_code() != 200)
                                throw std::runtime_error("Request failed: " + std::to_string(response.status_code()));

                            ucout << "Student was edited successfully" << std::endl;
                            return response.extract_json();
                        });
                break;
            }

            case 5: {
                string_t id;
                ucin >> id;

                auto request = client.request(methods::DEL, uri_builder(U("students")).append(id).to_string())
                        .then([](const http_response &response) {
                            if (response.status_code() == 404) {
                                std::cout << "Student not found" << std::endl;
                                pplx::cancel_current_task();
                            }

                            if (response.status_code() != 204)
                                throw std::runtime_error("Request failed: " + std::to_string(response.status_code()));

                            std::cout << "Student was removed successfully" << std::endl;
                        });
            }

            case 6:
                break;
        }
    }
    return 0;
}

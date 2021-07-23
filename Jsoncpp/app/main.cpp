// Copyright 2021 Mark Bond

#include <iostream>
#include <sstream>

#include <json/json.h>

void create()
{
    Json::Value root{};
    root["method"] = "authentication";
    root["server_password"] = "admin";
    root["client_id"] = "id1";

    std::cout << root << std::endl;
}

void parse()
{
    constexpr auto raw{
        R"(
            {
                "client_id": "id1",
                "unicode": "0",
                "msg": "6B656C6C6F",
                "reply": "proceeding",
                "number": "6453298",
                "id": 3432
            }
        )"
    };

    std::istringstream iss{raw};

    Json::Value root{};
    iss >> root;

    std::cout
        << "Client ID: "
        << root["client_id"].asString()
        << std::endl;
    std::cout
        << "Unicode: "
        << root["unicode"]
        << std::endl;
    std::cout
        << "Message: "
        << root["msg"]
        << std::endl;
    std::cout
        << "Reply: "
        << root["reply"]
        << std::endl;
    std::cout
        << "Number: "
        << root["number"]
        << std::endl;
    std::cout
        << "ID: "
        << root["id"]
        << std::endl;
}

int main()
{
    std::cout << "--- Create ---" << std::endl;
    create();

    std::cout << std::endl;

    std::cout << "--- Parse ---" << std::endl;
    parse();
}

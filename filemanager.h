//
// Created by ellaine on 2020-02-25.
//

#ifndef MSGQ_PRIORITY_FILEMANAGER_H
#define MSGQ_PRIORITY_FILEMANAGER_H

#include <iostream>
#include <fstream>

class filemanager {
public:
    filemanager() {};
    std::ifstream is;
    int openFile(std::string filename) {
        is.open(filename, std::ifstream::binary);
        if (is) {
            is.seekg(0, is.end);
            int length = is.tellg();
            is.seekg(0, is.beg);
            return length;
        } else {
            return -1;
        }
    };
    void read(char *buffer, int length) {
        is.read(buffer, length);
    };

};
#endif //MSGQ_PRIORITY_FILEMANAGER_H

#ifndef AUDITPARSES_H
#define AUDITPARSES_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define LOG_PATH "/var/log/audit/audit.log"           
#define OUTPUT_PATH "../tests/testAssets/testLog.txt"   

struct AuditEntry {
    std::string comm;
    std::string exe;
    std::string type;
    std::string name;   // path with name the file or derectory
    std::string nametype; // type of operation (CREATE, DELETE, etc.)
    std::string OUID;   // Owner User ID
    std::string OGID;   // Owner Group ID
    std::string cwd;    // File or directory name where command was executed
    std::time_t timestamp; // Time when the event occurred
};


class AuditParser { 
    public:
        std::string file_path = "/var/log/audit/audit.log";
        std::vector<std::string> dataBeforeParsing;
        //bool should_stop = false;  // ✅ FLAGA DO ZATRZYMANIA

        
        
        void loadData(){

            FILE *fp_in = fopen(LOG_PATH, "r");
            if (!fp_in) {
                perror("fopen LOG_PATH");
                return ;
            }

            FILE *fp_out = fopen(OUTPUT_PATH, "a");  
            if (!fp_out) {
               perror("fopen OUTPUT_PATH");
                fclose(fp_in);
                return ;
            }
            fseek(fp_in, 0, SEEK_END);

            char buffer[BUFFER_SIZE];
            size_t n;
            while (1) {
                
                n = fread(buffer, 1, BUFFER_SIZE, fp_in);
                std::cout<<n<<std::endl;
                
                
                if (n > 0) {
                    //if (fwrite(buffer, 1, n, fp_out) != n) {
                    //    perror("fwrite");
                    //    break;
                    //}
                    //fflush(fp_out);
                } else {

                    sleep(1);
                    clearerr(fp_in);  
                }
                
                if(n > 0){
                    std::cout<<"test1"<<std::endl;
                    std::string data(buffer, n);
                    
                    std::istringstream stream(data);
                    std::string line;
                    // audit entry
                    std::string cwd;
                    std::time_t timestamp;
                    std::string nametype;
                    std::string name;
                    std::string OUID;
                    std::string OGID;
                    
                    while (std::getline(stream, line)) {
                        if (!line.empty()) {
                            std::string newLine =line + "\n";
                            if (fwrite(newLine.c_str(), 1, newLine.length(), fp_out)) {
                                perror("fwrite output");
                            } else {
                                fflush(fp_out);  // Natychmiast zapisz na dysk
                                std::cout << "✅ Zapisano: " << nametype << " " << name << std::endl;
                            }
                            /*
                            if(line.find("type=CWD")==0){
                                cwd = line.substr(line.find("cwd=") + 4,line.find("msg=")-1);

                            }
                            if(line.find("msg=")!= std::string::npos and line.find("type=SYSCALL")==0) {
                                std::string msg = line.substr(line.find("msg=") + 4);
                                if (msg.find("audit") != std::string::npos) {
                                
                                    size_t start = msg.find("audit("+1) ;  // po "audit("
                                    size_t end = msg.find("):")-2;    
                                    if (start != std::string::npos && end != std::string::npos && end > start) {
                                        std::string timestamp_str = msg.substr(start, end);
                                        
                                        try {
                                            timestamp = std::stoll(timestamp_str);  
                                        } catch (const std::exception& e) {
                                            timestamp = std::time(nullptr);  
                                        }
                                    }         
                                }
                                
                            }
                            std::cout<<"cwd : "<<cwd<<std::endl;
                            if(line.find("type=PATH")==0 and (line.find("CREATE") or line.find("DELETE") or line.find("RENAME"))){
                                std::cout<<"wejscie do ifa"<<std::endl;
                                if(line.find("CREATE")){
                                    nametype = "CREATE";
                                }
                                else if(line.find("DELETE")){
                                    nametype = "DELETE";
                                }
                                else if(line.find("RENAME")){
                                    nametype = "RENAME";
                                }
                                //name = line.substr(line.find("name=") + 5 ,line.find("inode=")-2);
                                //std::cout<<"name : "<<name<<std::endl;
                                //std::cout<<"line : "<<line<<std::endl;
                                //OUID = line.substr(line.find("ouid=") + 5 ,line.find("ogid=")-2);
                                //OGID = line.substr(line.find("ogid=") + 5);
                                //std::string output_line = nametype; + ";" + name + ";" + OUID + ";" + OGID + ";" + cwd + "\n";
                                
                                
                            }
                            */
                        }
                    }
                }
            }

            fclose(fp_in);
            fclose(fp_out);
            return ;
        }
        
        //void processLine(const std::string& line);
    //public:
    //    AuditParser(const std::string& file_path);
    //    void parse();
    //    std::vector<std::string> getParsedData() const;
};

#endif
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
    std::string path;
    std::string name;
    std::string operation;
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

                    //sleep(1);
                    clearerr(fp_in);  
                }
                
                if(n > 0){
                    std::cout<<"test1"<<std::endl;
                    std::string data(buffer, n);
                    
                    std::istringstream stream(data);
                    std::string line;
                    // audit entry
                    std::string path;
                    std::string name;
                    std::string operation;
                    
                    while (std::getline(stream, line)) {
                        if (!line.empty()) {
                            //line = "type=PATH msg=audit(1754554010.721:135744): item=1 name=\"/dev/shm/.org.chromium.Chromium.gUA6SW\" inode=13197 dev=00:1b mode=0100600 ouid=1000 ogid=1000 rdev=00:00 nametype=DELETE cap_fp=0 cap_fi=0 cap_fe=0 cap_fver=0 cap_frootid=0 OUID=\"stas\" OGID=\"stas\"";
                            if(line.find("type=PATH")==0){
                                std::cout<<"line : "<<line<<std::endl;
                                if(line.find("nametype=PARENT")!= std::string::npos){
                                    std::cout<<"wejscie do ifa PARENT"<<std::endl;
                                    int start = line.find("name=") + 5;
                                    int end = line.find("inode=") - 2;
                                    path = line.substr(start, end-start);
                                }
                                else if(line.find("nametype=CREATE") != std::string::npos or line.find("nametype=DELETE") != std::string::npos or line.find("nametype=RENAME") != std::string::npos){
                                    std::cout<<line<<std::endl;
                                    if(line.find("CREATE") != std::string::npos){
                                        operation = "CREATE";
                                    }
                                    else if(line.find("DELETE") != std::string::npos){
                                        operation = "DELETE";
                                    }
                                    else if(line.find("RENAME") != std::string::npos){
                                        operation = "RENAME";
                                    }
                                    int startName = line.find("name=") + 5;
                                    int endName = line.find("inode=") - 2;
                                    name = line.substr(startName, endName-startName);
                                    
                                    std::string newLine =name + ";" + operation + ";" + path + "\n";
                                    std::cout << "Zapisuję linię: " << newLine << std::endl;
                                    //sleep(1000000000000000);
                                    if (fwrite(newLine.c_str(), 1, newLine.length(), fp_out)) {
                                        perror("fwrite output");
                                    } else {
                                        fflush(fp_out);  // Natychmiast zapisz na dysk
                                        //std::cout << "✅ Zapisano: " << nametype << " " << name << std::endl;
                                    }

                                }
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
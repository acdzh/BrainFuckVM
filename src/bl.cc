#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int DEBUGMODE = 0;
int sub = -1;
std::string subs(){
  std::string c = "";
  for(int i = 0; i< sub; i++) c+="****";
  return(c);
}

void ERROR(std::string why, int code = 0, int num = 255){
  std::cout << why;
  if(num == 255) std::cout << "!\n";
  else std::cout << ": " << num << "!\n";
  exit(code);
}

std::string toString(char c){
  std::string ascii[40] = {
    "NUL","SOH", "STX", "ETX", "EOT",  "ENQ", "ACK", "BEL", " BS", " HT", //0 - 9
    " \\n", " VT",  " FF",  " \\r",  " SO",   " SI", "DLE",  "DC1", "DC2", "DC3", //10 - 19
    "DC4", "NAK", "STN","ETB", "CAN",  " EM", "SUB",  "ESC", " FS",  " GS", //20 - 29
    " RS", " US", " "
  };
  if(c < 33) return ascii[c];
  if(c == 127) return "DEL";
  if(c > 127) return ("(" + std::to_string(int(c)), ")");
  else return std::string("") + c;
}

void myputchar(char c){
  std::cout << (c == '\n' || c == '\r' || c == '\b' ? std::string("") + c : toString(c));
}

void readFromFile(std::string &fileName, std::string &programText){
  std::ifstream file;
  std::string buffer = "";
  std::string temp = "";
  file.open(fileName);
  if (file.is_open()){
    while (!file.eof()){
      getline(file, buffer);
      for(int i = 0; i < buffer.size(); i++)
        if((buffer[i] == '/' && (i < buffer.size() - 1 && buffer[i+1] == '/')) || buffer [i] == '#'){
          buffer = buffer.substr(0, i);
          break;
        }
      if(DEBUGMODE) std::cout << buffer << std::endl;
      temp += buffer;
    }
    programText.clear();
    bool inStr = false;
    for(int i = 0; i < temp.size(); i++){
      int noteCount = 0;
      if(temp[i] == '/' && (i < temp.size() && temp[i+1] =='*')){
        noteCount ++;
        i += 2;
        for(i; noteCount; i++){
          if(i == temp.size() - 1) ERROR("Notes not close:", 3, noteCount);
          if(temp[i] == '/' && (i < temp.size() - 1 && temp[i+1] =='*')) noteCount++;
          if(temp[i] == '*' && (i < temp.size() - 1 && temp[i+1] =='/')) noteCount--;
        }
        i += 1;
        if(i == temp.size() - 1) break;
      }
      if(temp[i] == '"') inStr = !inStr;
      if((temp[i] != ' ' || (temp[i] == ' ' && inStr) )&& temp[i] != '\n' && temp[i] != '\r') programText += temp[i];
    }
    programText += "$$";
    if(DEBUGMODE) std::cout << temp << std::endl;
    if(DEBUGMODE) std::cout << programText << "\n";
    file.close();
  } 
  else ERROR("Read Files Error!", 1);
}

void printRam(std::vector<char> &RAM, char c = 'n'){
  std::vector<char>::iterator ramIndex = RAM.begin();
  if(DEBUGMODE) std::cout << "\n" << subs() << "DEBUG: |" << c << '|' ;
  for (int i = 0; i < 20; i++, ramIndex++) if(DEBUGMODE) std::cout << toString(*ramIndex) << "(" << int(*ramIndex) << "), ";
  if(DEBUGMODE) std::cout << *ramIndex << "(" << int(*ramIndex) << ")";
  if(DEBUGMODE) std::cout << "|\n";
}

void run(
  std::vector<char> &RAM,
  std::string &programText,
  std::string::iterator sIndex,
  std::string::iterator sEnd,
  std::vector<char>::iterator &ramIndex
){
  sub++;
  for(; sIndex != programText.end() && sIndex != sEnd; sIndex++){
    switch(*sIndex){
      case '$': break;
      case '>': ++ramIndex; printRam(RAM, '>'); break;
      case '<': --ramIndex; printRam(RAM, '<'); break;
      case '+': ++*ramIndex; printRam(RAM, '+'); break;
      case '-': --*ramIndex; printRam(RAM, '-'); break;
      case '.': myputchar(*ramIndex); printRam(RAM, '.'); break;
      case '?': std::cout << "(" << int(*ramIndex) << ")"; printRam(RAM, '?'); break;
      case ',':	*ramIndex = getchar(); while(getchar() != '\n'); printRam(RAM, ','); break;
      case '(': {
        std::string tempNumStr = "";
        for(sIndex++; *sIndex != ')'; sIndex++){
          if(!(*sIndex == 45 || *sIndex == 43 || (*sIndex > 47 && *sIndex < 58))) ERROR("Nums error", 4, *sIndex);
          if(*sIndex != '+') tempNumStr += *sIndex;
          if(sIndex == sEnd) ERROR("Nums not closed", 5);
        }
        bool nega = tempNumStr[0] == '-' ? true : false;
        if(nega) tempNumStr = tempNumStr.substr(1);
        int tempNum = 0;
        for(int i = 0; i < tempNumStr.size(); i++){
          if(tempNumStr[i] == '-') ERROR("Nums error", 4, tempNumStr[i]);
          else tempNum = tempNum * 10 + int(tempNumStr[i]) - 48;
        }
        *ramIndex = tempNum;
        printRam(RAM, ')');          
        break;
      }
      case '{': {
        std::string tempNumStr = "";
        for(sIndex++; *sIndex != '}'; sIndex++){
          if(!(*sIndex > 47 && *sIndex < 58)) ERROR("Print Nums error", 5, *sIndex);
          tempNumStr += *sIndex;
          if(sIndex == sEnd) ERROR("Nums not closed", 5);
        }
        int tempNum = 0;
        for(int i = 0; i < tempNumStr.size(); i++) tempNum = tempNum * 10 + int(tempNumStr[i]) - 48;
        auto tIndex = ramIndex;
        for(int i = 0; i < tempNum; i++, tIndex++) myputchar(*tIndex);
        printRam(RAM, '}');          
        break;
      }
      case '"': {
        sIndex++;
        *ramIndex = *sIndex;
        auto tIndex = ramIndex;
        while(*sIndex != '"'){
          if(sIndex != sEnd) {
            *tIndex = *sIndex;
            tIndex++;
            sIndex++;
          }
          else ERROR("char not close", 7);
        }
        printRam(RAM, '"');
        break;
      }
      case '[': {
        std::string::iterator subSEnd = sIndex;
        for (int bracketCounts = 0; subSEnd != sEnd; subSEnd++){
          if(*subSEnd == '[') bracketCounts++;
          if(*subSEnd == ']') bracketCounts--;
          if(bracketCounts == 0) {subSEnd; break;}
        }
        sIndex ++;
        for(;*ramIndex;sub--)run(RAM, programText, sIndex, subSEnd, ramIndex);
        sIndex = subSEnd++;
        printRam(RAM, ']');
        break;
      }
      default: std::cout << "Error char: " << *sIndex << " (" << int(*sIndex) << ")"; exit(2);
    }
  }
}

int main(int argc, char* argv[]){
	if(argc == 1) ERROR("please input filename", 7);
  else {
    std::vector<char> RAM(1000);
    std::string programText = ""; 
    for(int i = 1; i < argc; i++){
      if (argv[i][0] == '/' || argv[i][0] == '-'){
        if(argv[i][1] == 'd') DEBUGMODE = 1;
      } else {
        std::string path = argv[i];
        if(*(path.end() - 1) != 'f') path += ".bf";
        readFromFile(path, programText);
        std::string::iterator sIndex = programText.begin();
        std::string::iterator sEnd = programText.end() - 1;
        std::vector<char>::iterator ramIndex = RAM.begin();
        printRam(RAM);
        run(RAM, programText, sIndex, sEnd, ramIndex);
      }
    }
  }
  return 0;
}
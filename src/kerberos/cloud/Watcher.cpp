#include "cloud/Watcher.h"

namespace kerberos
{
    void Watcher::setup(const char * fileDirectory)
    {
        m_fileDirectory = fileDirectory;
    }
    
    void Watcher::addFile(const std::string & file)
    {
        std::ifstream f(file.c_str());
        if(f.good())
        {
            // Get filename
            std::vector<std::string> tokens;
            helper::tokenize(file, tokens, "/");
            std::string fileName = tokens[tokens.size()-1];

            // create a symbol link
            std::string link = SYMBOL_DIRECTORY + fileName;
            int beenCreated = symlink(file.c_str(), link.c_str());
            
            f.close();
        }
    }
    
    void Watcher::scan()
    {      
        while(true)
        {
            try
            {
                guard = new FW::Guard();
                guard->listenTo(m_fileDirectory);
                guard->onChange(&Watcher::addFile);
                guard->startLookingForNewFiles();
        
                while(true)
                {
                    guard->look();
                    usleep(2500*1000);
                }
            }
            catch(FW::FileNotFoundException & ex){}
            usleep(1000*1000);
        }
    }
}
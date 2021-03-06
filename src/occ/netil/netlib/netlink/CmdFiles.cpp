/* Software License Agreement
 * 
 *     Copyright(C) 1994-2019 David Lindauer, (LADSoft)
 * 
 *     This file is part of the Orange C Compiler package.
 * 
 *     The Orange C Compiler package is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     The Orange C Compiler package is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with Orange C.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *     contact information:
 *         email: TouchStone222@runbox.com <David Lindauer>
 * 
 */

#include <windows.h>
#include <io.h>
#include "CmdFiles.h"
using namespace std; // borland puts the io stuff in the std namespace...
                     // microsoft does not seem to.

const char *CmdFiles::DIR_SEP = "\\";
const char *CmdFiles::PATH_SEP = ";";

CmdFiles::~CmdFiles()
{
    for (auto name : names)
    {
        delete name;
    }
}
bool CmdFiles::Add(char **array, bool recurseDirs)
{
    while (*array)
    {
        Add(std::string(*array), recurseDirs);
        array++;
    }
    return true;
}
bool CmdFiles::RecurseDirs(const std::string &path, const std::string &name, bool recurseDirs)
{
    bool rv = false;
    struct _finddata_t find;
    std::string q = path + "*.*";
    long handle;
    // borland does not define the char * as const...
    if ((handle = _findfirst(const_cast<char *>(q.c_str()), &find)) != -1)
    {
        do
        {
            if (strcmp(find.name, ".") && strcmp(find.name, ".."))
            {
                if (find.attrib & _A_SUBDIR)
                {
                    std::string newName = path + std::string(find.name) + DIR_SEP + name;;
                    rv |= Add(newName, recurseDirs);
                }
            }
        }
        while (_findnext(handle, &find) != -1);
        _findclose(handle);
    }
    return rv;
}
bool CmdFiles::Add(const std::string &name, bool recurseDirs)
{
    bool rv = false;
    struct _finddata_t find;
    std::string path, lname;
    size_t n = name.find_last_of(DIR_SEP[0]);
    if (n != std::string::npos)
    {
        path = name.substr(0,n+1);
        lname = name.substr(n + 1);
    }
    else
    {
        n = name.find_last_of(':');
        if (n != std::string::npos)			
        {
            path = name.substr(0, n+1);
            lname = name.substr(n + 1);
        }
        else
        {
            lname = name;
        }
    }
    long handle;
    // borland does not define the char * as const...
    if ((handle = _findfirst(const_cast<char *>(name.c_str()), &find)) != -1)
    {
        do
        {
            if (!(find.attrib & _A_SUBDIR) && /*!(find.attrib & _A_VOLID) && */
                !(find.attrib & _A_HIDDEN))
            {
                std::string *file = new std::string(path + std::string(find.name));
                names.push_back(file);		
                rv = true;
            }
        }
        while (_findnext(handle, &find) != -1);
        _findclose(handle);
    }
    if (recurseDirs)
    {
        rv |= RecurseDirs(path, lname, recurseDirs);
    }
    if (!rv)
    {
        if (name.find_first_of('*')==std::string::npos && name.find_first_of('?') == std::string::npos)
        {
            std::string *newName = new std::string(name);
            names.push_back(newName);
        }
    }
    return rv;
}
bool CmdFiles::AddFromPath(const std::string &name, const std::string &path)
{
    bool rv = false;
    
    rv = Add(name, false);
    if (!rv)
    {
        size_t x = name.find_last_of(DIR_SEP[0]);
        if (x != std::string::npos)
        {
            x++ ;
        }
        else
        {
            x = name.find_first_of(":");
            if (x != std::string::npos)
                x++;
            else
                x = 0;
        }
        std::string internalName = name.substr(x, name.size());
        size_t n = 0;
        //size_t m = 0;
        bool done = false;
        while (!done)
        {
            size_t m = path.find_first_of(PATH_SEP, n);
            if (m == std::string::npos)
            {
                m = path.size();
                done = true;
            }
            std::string curpath = path.substr(m, n);
            n = m + 1;
            if (curpath.size() != 0 && curpath.substr(curpath.size()-1, curpath.size()) != DIR_SEP)
            {
                curpath += DIR_SEP;
            }
            curpath += internalName;
            rv = Add(curpath, false);
        }
    }
    return rv;
}
/**
Copyright 2014-2015 Jason R. Wendlandt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "Toolbox/FileSystem.h"

namespace alpha 
{
    char * OSGetBaseDirectory()
    {
        char * dir = (char *)malloc(sizeof(char) * (MAX_PATH + 1));
        memset(dir, 0, sizeof(dir));

        GetModuleFileName(NULL, dir, MAX_PATH);

        // remove the trailing file name, we only want the directory path
        for (int i = strlen(dir); i >= 0; --i)
        {
            char c = dir[i];
            if (c == '\\')
            {
                dir[i] = '\0';
                break;
            }
        }

        return dir;
    }

    char * OSJoinPath(const char * left, const char * right)
    {
        int leftLen = strlen(left) + 1;
        int rightLen = strlen(right) + 1;

        char * path = (char *)malloc(sizeof(char) * (leftLen + rightLen + 2));
        memset(path, 0, sizeof(path));

        strcat_s(path, leftLen + rightLen + 2, left);
        strcat_s(path, leftLen + rightLen + 2, "\\\0");
        strcat_s(path, leftLen + rightLen + 2, right);

        return path;
    }
}

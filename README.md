# Simple Shell - CMD clone
a simple console like cmd in window write in C

Submitted by: **Dat Nguyen**

Time spent: **30** hours spent in total
## User Stories

The following functionality is completed:

* [x] 'pwd': User can view their current directory
* [x] 'cd <path/foldername>': get to a specific directory according to 'path'
* [x] 'copy <path/filename> <foldername>': copy a file to a folder inside current directory
* [x] 'move <path/filename> <foldername>': move a file to a folder inside current directory
* [x] 'del <path/filename>': delete a specific file according to the path/filename
* [x] 'mkdir <path/foldername>': create a specific folder according to path/foldername
* [x] 'rd <path/foldername>': delete a folder no matter if it is empty or not
* [x] 'createfile <path/filename>': create new file, then if it is a *.txt file, user can text somthing and save it (included UNICODE characters)
* [x] 'viewfile <path/filename>': view an exist file, show its text to stdout (included UNICODE characters)
* [x] 'editfile <path/filename>': let user edit exits text file with 3 options:
      - 'replace <string_1> <string_2>': replace string_1 by string_2
      - 'insert <string_1> <string_2>': insert string_2 after string_1
      - 'close': return to main programe
* [x] 'exit': exit the programe

## Walkthrough 

Here's a walkthrough of implemented user stories:

<img src='http://s33.postimg.org/4jg6oby5b/copy.png' title='ex copy' width='' alt='Walkthrough' />
<img src='http://s33.postimg.org/kvq8e2cgv/create_and_view.png' title='ex create and view' width='' alt='Walkthrough' />
<img src='http://s33.postimg.org/z3kiggejj/edit.png' title='ex edit text file' width='' alt='Walkthrough' />


## License

    Copyright [2016] [Dat Nguyen]

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

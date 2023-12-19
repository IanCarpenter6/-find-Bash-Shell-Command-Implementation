# -find-Bash-Shell-Command-Implementation
Developed source code in C for my own version of the ‘find’ command used in a UNIX/Linux environment.
In this replication of the find command fulfilling the requirements that were asked, we will be utilizing the ‘a.out’ file given to the HPCC when compiling the program.

As you can see, my C file is called ‘find_program.c’. So we will use the following command to begin compilation:
![GitHub Image](/images/your-image.png)

Once the program is compiled, wwe will notice that our ‘a.out’ file is in the HPCC. For the following examples that will show my program meets all 4 requirements, we will use ‘./a.out’ for each command. Here is a demonstration of the where-to-look requirement that will display all files located in a specified directory:


Now, I will display the rest of the outputs regarding the last few requirements through example commands for each:

find where-to-look criteria:
-name:

-mmin:

-inum:

find where-to-look criteria -delete:


find where-to-look criteria -exec cat:

Inside the output.txt file:


find where-to-look criteria -exec rm:


find where-to-look criteria -exec mv: 
As seen in the screenshot, the txt file is renamed and reappears in the root directory


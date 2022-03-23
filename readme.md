# Archiver
This is my file archiver implementation. It uses canonical Huffman coding. 
This was written as a project for my university.

# Functionality
You can use this to archive and unarchive files. The following is the output for `archiver -h`:  
USAGE  
&nbsp;&nbsp;&nbsp;&nbsp;archiver -c archive_name file1 [file2 ...]  
&nbsp;&nbsp;&nbsp;&nbsp;archiver -d archive_name  
&nbsp;&nbsp;&nbsp;&nbsp;archiver -h  
DESCRIPTION  
&nbsp;&nbsp;&nbsp;&nbsp;use -c to compress files file1, file2, ... into an archive called archive_name  
&nbsp;&nbsp;&nbsp;&nbsp;use -d to decompress file archive_name back into original files  
&nbsp;&nbsp;&nbsp;&nbsp;use -h for help  

# Efficiency
Using this algorithm I was able to compress War and Peace by Leo Tolstoy from 4.3mb to just 2.2mb - almost half its original size.

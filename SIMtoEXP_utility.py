'''
Run this script with python3. Some functions are incompatible with python2.
'''

import sys, os, shutil


def read_header(f):
    '''Read the first line of .sim file, which should be the headers separated
    by a white space(s). Then, display the number of header words.'''
    line = f.readline()
    headers = line.split()
    print(len(headers))
    
  
def read_line(line):
    '''Read a line in .sim file and display the Z value and the last three 
    values, which are the water number densities. Return true if a non-zero
    value is present other than the last three; otherwise, return false.'''
    densities = line.split()
    z = densities.pop(0) # The Z value
    HW2 = densities.pop() # The second hydrogen in water
    HW1 = densities.pop() # The first hydrogen in water
    OW = densities.pop() # The oxygen in water
    
    # Check whether any non-zero value exists
    for i, val in enumerate(densities):
        if val != '0.000000':
            print("z OW HW1 HW2", z, OW, HW1, HW2, i, val, "True")
            return True
    #print("z OW HW1 HW2", z, OW, HW1, HW2, "False")
    return False


def read_lines(filename):
    '''display lines that contain a non-zero value other than
    the last three, which is water molecule. Ignore the first line, which is
    the header line. Use this to remove extra rows in the sim files.'''
    infile = open(filename, "r")
    header_line = infile.readline()
    rest_of_lines = infile.readlines()
    infile.close()
    
    if len(rest_of_lines) != 200:
        print(len(rest_of_lines))
        print("Wrong input file format!")
        sys.exit()
    
    for line in rest_of_lines:
        read_line(line)
           

def clean_up_header(name):
    fr = open(name, "r")
    first_line = fr.readline()
    headers = first_line.split()
    rest_of_lines = fr.readlines()
    fr.close()
  
    fw = open("tmp.sim", "w")
    for word in headers:
        print("{0:>15}".format(word), file=fw, end='')
    print("\n", file=fw, end='')
    for line in rest_of_lines:
        print(line, file=fw, end='')
    fw.close()
    
    shutil.move("tmp.sim", name) 


def replace_header(filename):
    '''Replace the header in the input file with the one from 
    schematics_and_names/header.txt'''
    infile = open(filename, "r")
    first_line = infile.readline()
    rest_of_lines = infile.readlines()
    infile.close()
    
    infile = open("schematics_and_names/header.txt", "r")
    header = infile.readline()
    infile.close()
    
    outfile = open("tmp.sim", "w")
    print(header, file=outfile)
    for line in rest_of_lines:
        print(line, file=outfile, end='')
    outfile.close()
    
    shutil.move("tmp.sim", filename) 
      

if __name__ == '__main__':
    #main()    
    # The length of sys.argv should be 2; otherwise, complain
    if len(sys.argv) == 1:
        print("Enter a filename. The syntax:\n",
              "python sim_file filename.sim")
        sys.exit()
    elif len(sys.argv) == 2:
        filename = sys.argv[1]
    else:
        print("Too many input arguments.")
        sys.exit()
    
    #replace_header(filename)
    read_lines(filename)

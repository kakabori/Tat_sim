import sys
            
def find_first_residue(r):
    for line in r:
        if line.startswith('; residue'):
            return line.split()[2]
            

def main():
    filename = sys.argv[1]
    input_file = open(filename, "r")
    output_file = open('header.txt', "w")
    count = 0
    residue = find_first_residue(input_file)
    for line in input_file:
        if line.startswith('; residue'):
            residue = line.split()[2]
        else:
            atom = line.split()[4]
            name = atom + '_' + residue + ' '
            output_file.write(name)
            count = count + 1
    input_file.close()
    output_file.close()
    print(count)

        
if __name__ == '__main__':
    main()

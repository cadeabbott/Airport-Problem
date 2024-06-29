import os

def save_directory_contents(output_filename="directory_contents.txt"):
    current_directory = os.getcwd()
    output_file_path = os.path.join(current_directory, output_filename)

    # Open the output file
    with open(output_file_path, "w") as output_file:
        # Iterate through all files in the current directory
        for filename in os.listdir(current_directory):
            # Exclude the output file and the script itself
            if filename == output_filename or filename == os.path.basename(__file__) or filename == 'airport_coordinates.txt':
                continue

            file_path = os.path.join(current_directory, filename)

            # Ensure it's a file and not a directory
            if os.path.isfile(file_path):
                # Write the filename
                output_file.write(f"Filename: {filename}\n")

                # Write the contents of the file
                with open(file_path, "r") as file:
                    lines = file.readlines()
                    output_file.writelines(lines)
                    output_file.write("\n" + "-"*40 + "\n")  # Separator between files

if __name__ == "__main__":
    save_directory_contents()

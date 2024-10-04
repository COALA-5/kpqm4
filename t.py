import os

l= []

def get_max_file_size(directory):
    max_size = 0
    for root, dirs, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            file_size = os.path.getsize(file_path)
            l.append(file_size)
    return l

if __name__ == "__main__":
    directory = "/bin"
    l = get_max_file_size(directory)
    max_size = max(l)
    print(f"The maximum file size in {directory} is {max_size} bytes.")
    print(0x40000)
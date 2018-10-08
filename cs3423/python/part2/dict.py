descriptions = {
    "/":        "root",
    "/home":    "home directories",
    "/bin":     "trash",
    "/sbin":    "system/administrative binaries",
    "/etc":     "configuration files",
    "/dev":     "device files",
    "/mnt":     "mounted file systems",
    "/var":     "system data and log files"
}


def main():
    some_dir = "/bin"

    print_summary(some_dir)

    # update an existin description
    descriptions["/bin"] = "system binaries"

    print_summary(some_dir)

    # add a new description
    descriptions["/boot"] = "boot-related files"

    print("\nPrinting directories whose description is not 'root'")
    for directory in descriptions.keys():
        if descriptions[directory] != "root":
            print(directory)

    print("\nPrinting all values")
    for description in descriptions.values():
        print(description)


def print_summary(path):
    print(path + " contains " + descriptions[path] + ".")

# check if this is the main program (not an import)
if __name__ == '__main__':
    main()

import os

# Define ANSI color codes
HEADER = "\033[99m"
OKBLUE = "\033[94m"
OKCYAN = "\033[96m"
OKGREEN = "\033[92m"
OKYELLOW = "\033[93m"
ADD = "\033[93m"  # Yellow plus for added comments and pragmas
BOLD = "\033[1m"
FAIL = "\033[91m"
ENDC = "\033[0m"


def update_files(directory):
    # Statistics
    stats = {
        "files_checked": 0,
        "pragmas_added": 0,
        "filenames_added": 0,
        "filenames_corrected": 0,
        "files_updated": 0,
        "file_actions": [],  # To store actions for each file
    }

    print(
        f"{HEADER}Checking header and source files for pragmas and filename comments...{ENDC}"
    )
    print(f"{OKBLUE}File Updates:{ENDC}")
    for root, _dirs, files in os.walk(directory):
        for file in files:
            if file.endswith((".h", ".cpp")):
                file_path = os.path.join(root, file)
                file_updated, actions = process_file(file_path, stats)
                if file_updated:
                    stats["files_updated"] += 1
                if actions:
                    stats["file_actions"].extend(actions)

    # Print the summary
    print_summary(stats)


def process_file(file_path, stats):
    file_updated = False
    actions = []
    try:
        with open(file_path, "w+") as file:
            stats["files_checked"] += 1
            lines = file.readlines()
            file.seek(0)
            file_name_comment = f"// {os.path.basename(file_path)}\n"

            # Determine if the file has #pragma once and file name comment
            pragma_once = "#pragma once"
            pragma_index = None
            comment_index = None

            # Check for existing #pragma once or file name comment
            for i, line in enumerate(lines):
                if pragma_once in line:
                    pragma_index = i
                if file_name_comment.strip() in line.strip():
                    comment_index = i

        # Add or update #pragma once and file name comment for .h files
        if file_path.endswith(".h"):
            if pragma_index is None:
                lines.insert(0, pragma_once + "\n")
                stats["pragmas_added"] += 1
                file_updated = True
                actions.append(f"{ADD}➕{ENDC} pragma once to {file_path}")

            if comment_index is None:
                lines.insert(0, file_name_comment)
                stats["filenames_added"] += 1
                file_updated = True
                actions.append(f"{ADD}➕{ENDC} filename comment to {file_path}")
            elif lines[comment_index].strip() != file_name_comment.strip():
                lines[comment_index] = file_name_comment
                stats["filenames_corrected"] += 1
                file_updated = True
                actions.append(f"✅ filename comment corrected in {file_path}")

            # Add or update file name comment for .cpp files
            elif file_path.endswith(".cpp"):
                if comment_index is None:
                    lines.insert(0, file_name_comment)
                    stats["filenames_added"] += 1
                    file_updated = True
                    actions.append(f"{ADD}➕{ENDC} filename comment to {file_path}")
                elif lines[comment_index].strip() != file_name_comment.strip():
                    lines[comment_index] = file_name_comment
                    stats["filenames_corrected"] += 1
                    file_updated = True
                    actions.append(f"✅ filename comment corrected in {file_path}")

            # Write the file back if updated
            if file_updated:
                file.truncate(0)  # Clear the file
                file.writelines(lines)

    except Exception as e:
        print(f"Error processing {file_path}: {e}")

    return file_updated, actions


def print_summary(stats):
    print(f"\n{OKBLUE}Process Summary:{ENDC}")
    print(f"{OKGREEN}┌─────────────────────┬───────┐{ENDC}")
    print(
        f"{OKGREEN}│{OKYELLOW} Files Checked       {OKGREEN}│{ENDC} {str(stats['files_checked']).rjust(5)} {OKGREEN}│{ENDC}"
    )
    print(
        f"{OKGREEN}│{OKYELLOW} Pragmas Added       {OKGREEN}│{ENDC} {str(stats['pragmas_added']).rjust(5)}{ENDC} {OKGREEN}│{ENDC}"
    )
    print(
        f"{OKGREEN}│{OKYELLOW} Comments Added      {OKGREEN}│{ENDC} {str(stats['filenames_added']).rjust(5)}{ENDC} {OKGREEN}│{ENDC}"
    )
    print(
        f"{OKGREEN}│{OKYELLOW} Comments Fixed      {OKGREEN}│{ENDC} {str(stats['filenames_corrected']).rjust(5)}{ENDC} {OKGREEN}│{ENDC}"
    )
    print(
        f"{OKGREEN}│{FAIL}{BOLD} Total Files Updated{OKGREEN} │{ENDC} {FAIL}{str(stats['files_updated']).rjust(5)}{ENDC} {OKGREEN}│{ENDC}"
    )
    print(f"{OKGREEN}└─────────────────────┴───────┘{ENDC}")


# Update files in the /lib directory
update_files("./lib/components")

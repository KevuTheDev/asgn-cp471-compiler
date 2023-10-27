


def check_file_extension(file_name: str, file_extension :str):
    file_extension_size = len(file_extension)

    if len(file_name) <= file_extension_size:
        return False

    if file_name[-file_extension_size:] != file_extension:
        return False

    return True
    
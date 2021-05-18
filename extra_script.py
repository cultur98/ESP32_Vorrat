from shutil import copyfile
from os import path

Import("env")

version_from_buildflags = "not_found"
my_flags = env.ParseFlags(env['BUILD_FLAGS'])
for x in my_flags.get("CPPDEFINES"):
    if isinstance(x, list):
        # grab as key, value
        k, v = x
        if k == "FW_VERSION_MAJ":
            fw_version_maj = v
        if k == "FW_VERSION_MIN":
            fw_version_min = v
        if k == "VERSION":
            version_from_buildflags = v

env.Replace(PROGNAME="esp32_vorrat_%s" % str(version_from_buildflags))
prg_name = env['PROGNAME']
print("PROGNAME: " + prg_name)
print("FW_VERSION: " + fw_version_maj + " dot " + fw_version_min)
version_file_name = "./bin/" + prg_name + ".version"
print(version_file_name)
fw=open(version_file_name,"w")
fw.write(fw_version_maj + "." + fw_version_min + "\n")
fw.close()
from linpgtoolbox.builder import Builder

# 需要额外包括的文件
additional_files: tuple[str, ...] = (r"../README.md", r"../LICENSE")

# 编译源代码
Builder.compile(
    "pyvns",
    additional_files=additional_files,
    update_the_one_in_sitepackages=True,
    include_pyinstaller_program=True,
    options={
        "enable_multiprocessing": True,
    },
)

# 提示编译完成
for i in range(2):
    print("")
print("--------------------Done!--------------------")
for i in range(2):
    print("")

# 打包上传最新的文件
"""
action: str = input("Do you want to package and upload the latest build (Y/n):")
if action == "Y":
    Builder.build()
    Builder.upload()
elif action != "N":
    Builder.remove("src")
"""
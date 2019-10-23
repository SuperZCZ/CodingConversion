# CodingConversion
文本编码转换小工具  
`git clone https://github.com/SuperZCZ/CodingConversion.git`
  
注意事项:
1. 根据本地IDE类型从Solution目录下复制工程文件至源码根目录
2. 打开解决方案或工程文件进行编译、修改
3. 提交时请将工程文件复制到Solution下对应目录后再进行提交，不要提交源码根目录下的工程文件
  
推荐使用:
	Visual Studio 2019 + Qt5.12.4
	Visual Studio 2015 + Qt5.6.0  
  
资源文件：
	本项目的资源文件和二进制可执行程序是相互独立的  
	资源文件统一存放于Resources目录下并按照配色进行分类  
	如果只是修改了资源文件可以不重新生成可执行程序,可以在工程目录下使用以下命令更新资源文件:  
	Debug:  
		`D:\Qt\Qt5.6.0\5.6\msvc2015_64\bin\rcc.exe --binary  -o .\x64\Debug\Resources\BlueTheme.rcc  .\Resources\BlueTheme\BlueTheme.qrc`
	Release:  
		`D:\Qt\Qt5.6.0\5.6\msvc2015_64\bin\rcc.exe --binary  -o .\x64\Release\Resources\BlueTheme.rcc  .\Resources\BlueTheme\BlueTheme.qrc`

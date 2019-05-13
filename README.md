# ComputerGraphicsHW

总项目地址：[ComputerGraphicesHW](/ComputerGraphicsHW/)

环境：

​	Visual Studio 2017

​	OpenGL 3.2

​	GLFW 3.2.1

## VS项目属性配置

- 常规

  - 输出目录&中间目录

    ```
    $(ProjectDir)$(Configuration)\
    ```

- VC++ 目录

  - 包含目录：

    ````
    ..\OpenGLStuff\include
    ````

  - 库目录：

    ````
    ..\OpenGLStuff\lib
    ````

- C/C++

  - 附加包含目录

    ```
    ..\OpenGLStuff\include\imgui
    ..\OpenGLStuff\include\imgui\examples
    ..\OpenGLStuff\include\imgui\examples\libs\gl3w
    ```

- 链接器

  - 输入 - 附加依赖项：

    ```
    glfw3.lib
    opengl32.Lib
    ```

    


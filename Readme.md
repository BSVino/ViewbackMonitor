Viewback Monitor
================

This is the [Viewback](https://github.com/BSVino/Viewback) monitor implementation. It's a frontend that implements the Viewback client API. If you're a typical user of Viewback then you probably only need to concern yourself with the Viewback server and client code, and not this repository.

The Viewback Monitor repository is separate from the Viewback repository because the monitor has a ton of extra code and dependencies that Viewback itself doesn't need.

It currently compiles on Windows, Linux, and Android. OSX and iOS ports are in the works.

Compiling
---------

This project can be tricky to compile. I will not be providing step-by-step instructions, you shouldn't attempt this if you don't have some programming and specifically C++ experience. It relies on the following libraries.

* EASTL found here: https://github.com/paulhodge/EASTL (optional, can be removed with the WITH_EASTL CMake option set to NO or FALSE or 0 or whatever.)
* Freetype
* FTGL-GL3 found here: https://github.com/BSVino/ftgl-gl3
* SDL2
* SDL2_image
* Google Protocol Buffers found here: https://code.google.com/p/protobuf/downloads/list
* GL3W (win32 only) found here: https://github.com/skaslev/gl3w
* pthreads-w32 (win32 only) found here: http://sourceware.org/pthreads-win32/

For Linux you can mostly get away with installing these packages with your package manager, but some packages may need manual installing. For Windows you must download these packages and place them in a directory named ext-deps, which is where Tinker will look for them when you compile.

At the moment the only supported compiler on Windows is Visual Studio 2013. All statically linked dependencies (which is all except SDL and SDL_image) will have to be compiled with 2013 as well.

Tinker uses CMake to generate its project files. Download and install the latest version of CMake (make sure to add it to your environment path) and create a "build" directory. (Google "out of source builds" for information.) Go to that source directory and run the following command:

	cmake c:\path\to\source\

or for Linux

	cmake /path/to/source

This will create makefiles or Visual Studio project files. The majority of Tinker is compiled statically. That is the supporting libraries except SDL are linked in statically, and Tinker itself is a statically linked application which is linked statically into your project. In many cases this requires rebuilding the supporting libraries.

Android build instructions are a little different and you'll find them inside the build.bat in the `android` directory.

Tinker
------

The Viewback monitor uses Tinker as a GUI and backend. Tinker is Lunar Workshop's internal engine, used to ship the game Digitanks. Here the engine parts have been stripped away and only the GUI and application code remain.

### Overview

`common` - These files are general utility files. tstring.h is important - it defines a tstring which is the string format you should use for applications based on Tinker. Avoid using windows.h or other platform-specific libraries, use platform.h from this directory instead.

`datamanager` - Sometimes data needs to be serialized to and from the hard drive. data.h contains a CData class that is a generic data container, and dataserializer.h can serialize any CData to and from a stream. Data is stored in a simple format that looks like this:

	key: value
	{
		// Sub-datas
		key: value
		key2: value

		// Values are optional
		key3
		{
			subdata
		}
	}

`glgui` - This contains files for a GUI that renders directly to OpenGL. Tinker will automatically create the root panel for this GUI. To create a control use this syntax:

	glgui::CControl<glgui::CButton> hButton = AddControl(new glgui::CButton());

(Apologies for the Hungarian notation, I've outgrown it since but there's still all this code written with it, you know how that is.) The CControl structure is a smart pointer, once the panel removes the control and all CControl handles are destroyed the control will automatically free itself. Don't create a control without using this pattern.

The Layout() function is responsible for laying out items on a screen. Mostly it should be used to set positions and sizes for child controls or itself. Layout is usually called when a panel is created, resized or changed somehow. Each control has some helper layout functions that start with `Layout_*` for example `Layout_FullWidth()`.

`tinker` - Application classes. The creator of a project is responsible for the main() function of her application. Subclass the CApplication class and call OpenWindow() and then Run() to start your application.





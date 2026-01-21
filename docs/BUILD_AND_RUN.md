# How to Build & Run the Project

This guide explains how to set up, build, and run the **XML Social Network Analyzer** using **VS Code**, **Qt 6**, and **CMake**.

## Required Tools (Install Before Running)

|Tool|Purpose|Download Link|
|---|---|---|
|**Qt 6+**|GUI framework|[https://www.qt.io/](https://www.qt.io/)|
|**CMake**|Build system|[https://cmake.org/download/](https://cmake.org/download/)|
|**VS Code Extensions**|Required inside VS Code|- _Qt Tools_  <br>- _CMake Tools_|

## Clone the Repository

Open terminal and run:

```sh
git clone https://github.com/amr10w/XML-Social-Network-Analyzer.git
```

Then open the repository folder in **VS Code**.

## Step 1 — Register Qt in VS Code

1. Press **Ctrl + Shift + P**
2. Search: **Qt: Register Qt Installation**
3. Choose the Qt folder you installed (for example):

```sh
D:\Qt
```

- This allows the VS Code Qt extension to detect `qmake`, Qt libraries, and tools.

## Step 2 — Configure CMake Tools

1. Press **Ctrl + Shift + P**
2. Search: **CMake: Quick Start**  
	(only the first time to let CMake configure the project)
3. Press **Ctrl + Shift + P** again
4. Search: **CMake: Select a Kit**
5. Choose the kit that corresponds to **Qt 6** (e.g., _minjw or _MSVC + Qt_ or _GCC + Qt_).

## Step 3 — Build & Run the Project

### ▶️ Run in GUI Mode (Normal Application Run)

Try:

```sh
Ctrl + Shift + F5
```

If this works, the GUI will start directly.

### If Ctrl+Shift+F5 doesn’t work:

You can:

#### Option A — Build using CMake Tools

1. Click **Build** (bottom status bar in VS Code)
2. When build finishes, you can run the target from VS Code.

#### Option B — Create a debug configuration

1. Press **Ctrl + Shift + D**
2. Click **Create launch.json**
3. Choose:

```nginx
Qt: Debug with cppvsdbg (Windows)
```

Then run using:

`Ctrl + Shift + F5`

This always starts the **GUI Mode**.

---

## Running in Command Line Mode (CLI)

After the first GUI run, your build folder will contain `ilovexml.exe`.

To run CLI mode:

```sh
cd .\build\ 
.\ilovexml.exe <command>
```

> **Note:** For specific CLI commands, refer to the [Main README](../README.md#usage).

## Installer

To facilitate seamless deployment, the development team utilized Inno Setup to generate a standalone executable installer (.exe). This installation package efficiently bundles all required Dynamic Link Libraries (DLLs) using high-ratio compression, ensuring that the software has all necessary runtime dependencies without requiring manual configuration by the user.

[**Download Installer**](https://github.com/amr10w/XML-Social-Network-Analyzer/tree/main/installation)

Furthermore, the installation script is configured to automatically append the application’s binary directory to the Windows PATH environment variable. This critical configuration enables global command-line accessibility, allowing the user to invoke the CLI tool simply by typing `ilovexml` from any command prompt location, thereby eliminating the need to reference the absolute file path.

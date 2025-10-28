# Mac External Device Ejector

A C language application for safely ejecting all external devices on macOS, including USB drives, external hard drives, SD cards, and more.

## Features

- 🔄 **Multiple Ejection Methods**: Uses 4 different approaches to ensure devices are safely ejected
- 🛡️ **Safety Protection**: Automatically identifies and skips system volumes to prevent accidental system unmounting
- 📊 **Detailed Output**: Shows operation process and result statistics
- ⚡ **Efficient Execution**: Parallel processing of multiple devices for improved efficiency

## Supported Device Types

- ✅ USB flash drives
- ✅ External hard drives
- ✅ SD card readers
- ✅ Disk images (.dmg)
- ✅ Network shared volumes
- ❌ System main hard drive (automatically protected)
- ❌ System recovery partitions
- ❌ Time Machine backup volumes

## Usage

### Compile the Program

```bash
gcc -Wall -Wextra -std=c99 -O2 -o eject_devices eject_devices.c
```

### Run the Program

```bash
./eject_devices
```

### Install to System Path (Optional)

```bash
sudo cp eject_devices /usr/local/bin/
sudo chmod +x /usr/local/bin/eject_devices
```

After installation, you can run it from anywhere:

```bash
eject_devices
```

## Program Execution Flow

1. **Scan Disks**: Get a list of all connected disk devices
2. **Identify External Devices**: Check device properties using `diskutil info` command
3. **Multiple Ejection Strategies**:
   - Use `diskutil unmount/eject` commands
   - Use AppleScript through Finder
   - Check and eject volumes mounted under `/Volumes/`
   - Use `hdiutil` to force eject disk images
4. **Result Display**: Show the number of successfully ejected devices

## Technical Implementation

### Core Functions

- `execute_command()`: Execute shell commands and return results
- `is_external_device()`: Check if a device is external
- `eject_device()`: Eject a single device
- `eject_with_applescript()`: Eject all devices using AppleScript
- `eject_mounted_volumes()`: Eject all mounted volumes

### Safety Mechanisms

The program ensures system safety through:

1. **Device Property Check**: Check `External` and `Removable` properties
2. **System Volume Filtering**: Skip devices containing `disk0` and `Macintosh HD`
3. **Multiple Verification**: Confirm device types through various methods

## Output Example

```
=== Mac External Device Ejector ===
Scanning for ejectable devices...

Ejecting device: disk2
Using AppleScript to eject devices...
Checking mounted volumes...
Using hdiutil for forced ejection...

=== Operation Complete ===
Attempted to eject 2 external devices
Note: System volumes are protected, only genuine external devices are processed.
```

## System Requirements

- macOS 10.12 or later
- Terminal access required
- Administrator privileges may be needed for certain operations

## Important Notes

1. **Data Safety**: Ensure no files are actively being used on the devices
2. **Permission Requirements**: Some operations may require password input
3. **Network Volumes**: Network shared volumes may not eject properly
4. **Time Machine**: Time Machine volumes currently backing up may not eject

## Troubleshooting

If the program fails to eject devices properly, try:

1. Check if devices are being used by other applications
2. Manually use `diskutil list` to check device status
3. Ensure you have sufficient permissions to perform operations

## Compilation Options

### Basic Compilation
```bash
gcc -o eject_devices eject_devices.c
```

### With All Warnings
```bash
gcc -Wall -Wextra -o eject_devices eject_devices.c
```

### Optimized Build
```bash
gcc -O2 -o eject_devices eject_devices.c
```

### Debug Build
```bash
gcc -g -o eject_devices eject_devices.c
```

## Code Structure

```
eject_devices.c
├── Includes and headers
├── execute_command() - Shell command execution
├── is_external_device() - Device type detection
├── eject_device() - Single device ejection
├── eject_with_applescript() - Finder-based ejection
├── eject_mounted_volumes() - Volume management
└── main() - Program entry point and flow control
```

## License

This program is open source software, free to use and modify.
# Mac 外置设备弹出程序

这是一个用C语言编写的Mac应用程序，用于安全弹出所有外置设备，包括USB驱动器、外部硬盘、SD卡等。

## 功能特性

- 🔄 **多重弹出方法**：使用4种不同方法确保设备被安全弹出
- 🛡️ **安全保护**：自动识别并跳过系统卷，避免意外卸载系统
- 📊 **详细输出**：显示操作过程和结果统计
- ⚡ **高效执行**：并行处理多个设备，提高弹出效率

## 支持的设备类型

- ✅ USB闪存驱动器
- ✅ 外部硬盘
- ✅ SD卡读卡器
- ✅ 磁盘镜像(.dmg)
- ✅ 网络共享卷
- ❌ 系统主硬盘（自动保护）
- ❌ 系统恢复分区
- ❌ Time Machine备份卷

## 使用方法

### 编译程序

```bash
gcc -Wall -Wextra -std=c99 -O2 -o eject_devices eject_devices.c
```

### 运行程序

```bash
./eject_devices
```

### 安装到系统路径（可选）

```bash
sudo cp eject_devices /usr/local/bin/
sudo chmod +x /usr/local/bin/eject_devices
```

安装后，您可以在任何位置直接运行：

```bash
eject_devices
```

## 程序执行流程

1. **扫描磁盘**：获取所有连接的磁盘设备列表
2. **识别外部设备**：通过`diskutil info`命令检查设备属性
3. **多重弹出策略**：
   - 使用`diskutil unmount/eject`命令
   - 使用AppleScript通过Finder弹出
   - 检查并弹出挂载在`/Volumes/`下的卷
   - 使用`hdiutil`强制弹出镜像文件
4. **结果显示**：显示成功弹出的设备数量

## 技术实现

### 核心函数

- `execute_command()`: 执行shell命令并返回结果
- `is_external_device()`: 检查设备是否为外部设备
- `eject_device()`: 弹出单个设备
- `eject_with_applescript()`: 使用AppleScript弹出所有设备
- `eject_mounted_volumes()`: 弹出所有挂载的卷

### 安全机制

程序通过以下方式确保系统安全：

1. **设备属性检查**：检查`External`和`Removable`属性
2. **系统卷过滤**：跳过包含`disk0`和`Macintosh HD`的设备
3. **多重验证**：通过多种方法确认设备类型

## 输出示例

```
=== Mac 外置设备弹出程序 ===
正在扫描可弹出的设备...

正在弹出设备: disk2
使用AppleScript弹出设备...
检查挂载的卷...
使用hdiutil强制弹出...

=== 操作完成 ===
已尝试弹出 2 个外部设备
注意: 系统卷不会被弹出，只有真正的外部设备会被处理。
```

## 系统要求

- macOS 10.12 或更高版本
- 需要终端访问权限
- 可能需要管理员权限执行某些操作

## 注意事项

1. **数据安全**：确保设备没有正在使用的文件
2. **权限要求**：某些操作可能需要输入密码
3. **网络卷**：网络共享卷可能无法正常弹出
4. **Time Machine**：正在备份的Time Machine卷可能无法弹出

## 故障排除

如果程序无法正常弹出设备，请尝试：

1. 检查设备是否被其他程序占用
2. 手动使用`diskutil list`查看设备状态
3. 确保有足够的权限执行操作

## 许可证

此程序为开源软件，可自由使用和修改。
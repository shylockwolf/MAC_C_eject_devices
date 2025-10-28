#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 执行shell命令并返回结果的函数
char* execute_command(const char* command) {
    FILE *fp;
    char buffer[1024];
    static char result[4096] = "";
    
    fp = popen(command, "r");
    if (fp == NULL) {
        return "";
    }
    
    result[0] = '\0';
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        strcat(result, buffer);
    }
    
    pclose(fp);
    return result;
}

// 检查设备是否为外部设备
int is_external_device(const char* disk) {
    char command[256];
    char* result;
    
    // 检查是否为外部设备
    snprintf(command, sizeof(command), "diskutil info %s 2>/dev/null | grep -c 'External.*Yes'", disk);
    result = execute_command(command);
    if (atoi(result) > 0) {
        return 1;
    }
    
    // 检查是否为可移动设备
    snprintf(command, sizeof(command), "diskutil info %s 2>/dev/null | grep -c 'Removable.*Yes'", disk);
    result = execute_command(command);
    if (atoi(result) > 0) {
        return 1;
    }
    
    return 0;
}

// 弹出单个设备
void eject_device(const char* disk) {
    char command[256];
    
    printf("正在弹出设备: %s\n", disk);
    
    // 先尝试卸载
    snprintf(command, sizeof(command), "diskutil unmount %s 2>/dev/null", disk);
    system(command);
    
    // 再尝试弹出
    snprintf(command, sizeof(command), "diskutil eject %s 2>/dev/null", disk);
    system(command);
    
    // 短暂延迟
    usleep(100000); // 100ms
}

// 使用AppleScript弹出所有设备
void eject_with_applescript() {
    printf("使用AppleScript弹出设备...\n");
    
    const char* applescript = 
    "osascript -e 'tell application \"Finder\"' "
    "-e 'try' "
    "-e 'eject (every disk whose ejectable is true)' "
    "-e 'end try' "
    "-e 'end tell' 2>/dev/null";
    
    system(applescript);
}

// 弹出所有挂载的卷
void eject_mounted_volumes() {
    printf("检查挂载的卷...\n");
    
    FILE *fp = popen("mount | grep '/Volumes' | awk '{print $1}'", "r");
    if (fp != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), fp) != NULL) {
            // 移除换行符
            line[strcspn(line, "\n")] = 0;
            
            // 跳过系统卷
            if (strstr(line, "disk0") != NULL || strstr(line, "Macintosh HD") != NULL) {
                continue;
            }
            
            printf("正在弹出卷: %s\n", line);
            
            char command[256];
            snprintf(command, sizeof(command), "diskutil unmount \"%s\" 2>/dev/null", line);
            system(command);
        }
        pclose(fp);
    }
}

int main() {
    printf("=== Mac 外置设备弹出程序 ===\n");
    printf("正在扫描可弹出的设备...\n\n");
    
    // 获取所有磁盘列表
    FILE *fp = popen("diskutil list | grep -o 'disk[0-9][0-9]*' | sort -u", "r");
    if (fp == NULL) {
        printf("错误: 无法获取磁盘列表\n");
        return 1;
    }
    
    char disk[32];
    int ejected_count = 0;
    
    // 处理每个磁盘
    while (fgets(disk, sizeof(disk), fp) != NULL) {
        // 移除换行符
        disk[strcspn(disk, "\n")] = 0;
        
        // 跳过空字符串
        if (strlen(disk) == 0) {
            continue;
        }
        
        // 检查是否为外部设备
        if (is_external_device(disk)) {
            eject_device(disk);
            ejected_count++;
        }
    }
    pclose(fp);
    
    // 使用AppleScript方法
    eject_with_applescript();
    
    // 弹出挂载的卷
    eject_mounted_volumes();
    
    // 使用hdiutil强制弹出
    printf("使用hdiutil强制弹出...\n");
    system("hdiutil eject /Volumes/* -force 2>/dev/null");
    
    printf("\n=== 操作完成 ===\n");
    printf("已尝试弹出 %d 个外部设备\n", ejected_count);
    printf("注意: 系统卷不会被弹出，只有真正的外部设备会被处理。\n");
    
    return 0;
}
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import re
from collections import deque
import threading
import time

# --- 配置参数 ---
SERIAL_PORT = '/dev/ttyUSB0'  # 请根据实际情况修改端口号，例如 'COM3' 或 '/dev/ttyUSB0'
BAUD_RATE = 115200
MAX_POINTS = 200  # 图表显示的并在内存保留的最大数据点数
TARGET_SPEED = 300.0

# --- 数据容器 ---
times = deque(maxlen=MAX_POINTS)
speeds = deque(maxlen=MAX_POINTS)

# 全局变量
start_time = time.time()
data_lock = threading.Lock()

def read_serial():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"成功打开串口: {SERIAL_PORT}")
    except serial.SerialException as e:
        print(f"无法打开串口 {SERIAL_PORT}: {e}")
        return

    pattern = re.compile(
        r"Speed:\s*([-+]?\d*\.?\d+)\s*mm/s"
    )

    while True:
        try:
            if ser.in_waiting:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                match = pattern.search(line)
                if match:
                    speed = float(match.group(1))
                    
                    current_time = time.time() - start_time
                    
                    with data_lock:
                        times.append(current_time)
                        speeds.append(speed)
                        
                    # print(f"Parsed: Time={current_time:.2f}, speed={speed}, pwm={pwm}")
        except Exception as e:
            print(f"读取数据错误: {e}")
            time.sleep(0.1)

def update_plot(frame):
    ax.clear()

    with data_lock:
        if not times:
            ax.set_title('Real-time Motor Speed')
            ax.set_xlabel('Time (s)')
            ax.set_ylabel('Speed (mm/s)')
            ax.text(0.5, 0.5, '等待串口数据...', ha='center', va='center', transform=ax.transAxes)
            ax.grid(True)
            return
        
        t = list(times)
        s = list(speeds)
    
    ax.plot(t, s, label='Current Speed', color='blue', linewidth=2)
    
    ax.axhline(y=TARGET_SPEED, color='green', linestyle='--', label=f'Target Speed ({TARGET_SPEED:.0f})')

    ax.set_title('Real-time Motor Speed')
    ax.set_xlabel('Time (s)')
    ax.set_ylabel('Speed (mm/s)')
    ax.legend(loc='upper left')
    ax.grid(True)

if __name__ == '__main__':
    # 启动串口读取线程
    serial_thread = threading.Thread(target=read_serial, daemon=True)
    serial_thread.start()

    # 设置绘图
    fig, ax = plt.subplots(figsize=(10, 5))
    ani = animation.FuncAnimation(fig, update_plot, interval=100, cache_frame_data=False) # 100ms 更新一次
    plt.tight_layout()

    plt.show()
# 我现在在调直流电机pid,但是现在的pid目标速度超过240后就没法稳定，目标速度设为300时，会出现振荡特别大，误差能达到接近正负100，pi调大了后电机一上电速度就特别大，并且编码器打印的脉冲几乎不变，并且很小，就像是速度大了后编码器就检测不到变化了，导致pid输出的值一直是最大
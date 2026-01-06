# PendulumLab
Physics Simulator for Single and Multiple Pendulums Based on C++

## 一、项目内容

### 1. 项目名称
单摆和多摆物理模拟器。

### 2. 基础功能
模拟单摆的运动过程。支持用户调整摆球的质量（1.00kg-10.00kg)、摆长（1.00m-5.00m）、初始摆角（1.00°-60.00°）和空气阻力系数（0.01-0.05）。参数设置完毕后，单击“开始模拟”即可展示单摆的运动过程。模拟时，终端左侧会实时展示系统动能、系统势能和机械能，同时用户可以随时点击“暂停”按键来中止运动过程，单击“继续”后模拟继续进行。

* (1) **空气阻力计算公式**：*f* = *kv*²，其中 *f* 是空气阻力的大小，*k* 是阻力系数，*v* 是摆球运动速度的大小；
* (2) **系统动能的计算公式**：*Eₖ* = *mv*²/2，其中 *Eₖ* 是单个摆球的动能，*m* 是单个摆球的质量，*v* 是单个摆球运动速度的大小；
* (3) **系统势能的计算公式**：*Eₚ* = *mgh*，其中 *Eₚ* 是单个摆球的势能，这里选取摆线自然悬挂时的最低点为零势能面，*m* 是单个摆球的质量，*g* 是重力加速度（取 9.8m/s²）, *h* 是单个摆球当前所处的位置与零势能面在竖直方向上的高度差；
* (4) **机械能的计算公式**：*E* = *Eₖ* + *Eₚ*，其中 *Eₖ* 是单个摆球的动能，*Eₚ* 是单个摆球的势能。

### 3. 进阶功能
模拟多摆的运动过程。操作方法基本同上，用户可在此基础上调整摆球的数量（1~10）。

* (1) 对于每个小球，都有 *f* = *kv*²，*v* 是每个小球运动速度的大小；
* (2) **系统动能的计算公式**：*Eₖ* = Σ*mv*²/2，其中 *Eₖ* 是多个摆球的总动能，*m* 是每个摆球的质量，*v* 是每个小球运动速度的大小；
* (3) **系统势能的计算公式**：*Eₚ* = Σ*mgh*，其中 *Eₚ* 是多个摆球的总势能，这里选取摆线自然悬挂时的最低点为零势能面，*m* 是每个小球的质量，*g* 是重力加速度（取 9.8m/s²）, *h* 是每个小球当前所处的位置与零势能面在竖直方向上的高度差；
* (4) **机械能的计算公式**：*E* = *Eₖ* + *Eₚ*，其中 *Eₖ* 是多个摆球的总动能，*Eₚ* 是多个摆球的总势能。

---

## 二、子函数说明

### 1. pendulumwidget.h
定义类的框架，提供接口，同时声明信号（signals）和槽函数（slots），用于 Qt 的事件通信机制。

### 2. main.cpp
程序的入口，模拟器的启动器，打开动画窗口。

### 3. pendulumwidget.cpp
单摆和多摆物理模拟器的具体实现。

* (1) `PendulumWidget::PendulumWidget(QWidget *parent)`：构造函数，初始化界面布局、控件（旋钮/按钮）、计时器，并设置默认摆球参数；
* (2) `PendulumWidget::~PendulumWidget()`：析构函数，停止计时器，同时防止内存泄漏；
* (3) `void PendulumWidget::initializePendulums()`：根据用户给定的参数初始化摆球位置和初始能量；
* (4) `void PendulumWidget::updatePhysics()`：逐帧计算摆球的运动状态，更新位置并约束摆长；
* (5) `void PendulumWidget::applyConstraints()`：强制约束摆球间距固定，同时保证物理结果的合理性；
* (6) `void PendulumWidget::calculateEnergies(qreal &kinetic, qreal &potential) const`：实时计算系统总动能、系统势能和机械能，并用于界面展示；
* (7) `void PendulumWidget::paintEvent(QPaintEvent *)`：绘制摆线、摆球、固定点，并显示能量等参数；
* (8) `void PendulumWidget::startSimulation()`：重置摆球并启动模拟，触发计时器；
* (9) `void PendulumWidget::pauseSimulation()`：切换暂停/继续状态，控制计时器启停；
* (10) `void PendulumWidget::onStartClicked()`：单击“开始”按键时调用 `void PendulumWidget::startSimulation()`；
* (11) `void PendulumWidget::onPauseClicked()`：点击“暂停/继续”按钮时调用 `void PendulumWidget::pauseSimulation()`；
* (12) `void PendulumWidget::resizeEvent(QResizeEvent *event)`：窗口大小改变时重新初始化摆球以保持比例；
* (13) `void PendulumWidget::showEvent(QShowEvent *event)`：窗口首次显示时初始化摆球。

---

## 三、使用指南

> **注意**：请仔细阅读下面的内容，并严格按照说明执行相应的操作，这对于正确应用该项目至关重要。
>
> 如若演示者未安装 Qt Creator 6.9.0，请遵照下面的指示自行安装；如若演示者已安装 Qt Creator 6.9.0，请跳至本部分“4.项目导入”继续阅读。

### 1. 软件下载
* (1) 使用浏览器（建议选择 Google Chrome 或 Edge）前往 [https://www.qt.io/](https://www.qt.io/) ，单击右上角 "Download. Try."；
* (2) 页面右侧出现 "Community User" 一栏，单击 "Explore Qt Community Edition"；
* (3) 下拉页面直至出现 "Get Started" 字样，单击其上方的 "Download the Qt Online Installer"；
* (4) 根据操作系统选择合适的版本，下载即可。

### 2. 账号注册
* (1) 在上一步的界面 [https://www.qt.io/download-qt-installer-oss](https://www.qt.io/download-qt-installer-oss) 单击用户图标；
* (2) 单击 "Create Qt Account"，创建一个账号；
* (3) 输入邮箱地址并创建密码，验证密码，输入验证码，勾选 "I accept the Qt Account terms."，单击 "Create Qt Account"；
* (4) 当页面显示 "Your Qt Account has been created." 后，在刚刚填写的邮箱中找到 Qt Creator 自动发送的邮件，点击邮件中的链接以激活账号。

### 3. 本地安装
* (1) 双击打开刚才下载的 `.exe` 安装器，登录账户，点击“下一步”；
* (2) 勾选“我已阅读并同意使用开源 Qt 的条款和条件”和“我是个人用户，我不为任何公司使用 Qt”，**请勿在方框中输入任何内容**，否则将无法免费使用；
* (3) 等待程序从远程仓库检索信息，检索完毕后页面将自动跳转，用户自行选择 "Help us to improve by enabling sending pseudonymous usage statistics in Qt Creator" 或 "Disable sending pseudonymous usage statistics in Qt Creator"；
* (4) 指定安装目录（不建议选择 C 盘），**特别注意，安装路径中不能出现中文**，这可能会导致 Qt Creator 无法正常运行；
* (5) 勾选安装组件，确保选择的磁盘中有足够的空间来安装 Qt Creator。

### 4. 项目导入
* (1) 双击打开 Qt Creator，依次单击左上角“文件(F)”、“打开文件或项目(O)”；
* (2) 选择文件夹 PendulumLab 中的 `CMakeLists.txt`，单击打开；
* (3) 双击屏幕左侧的具有文件夹图标的 "PendulumLab" 以展开，双击具有榔头图标的 "PendulumLab" 再次展开，依次双击 "Header Files" 和 "Source Files"，可以看到如下界面：

```text
PendulumLab
 ├── CMakeLists.txt
 └── PendulumLab
    ├── Header Files
    │  └── pendulumwidget.h
    └── Source Files
       ├── main.cpp
       └── pendulumwidget.cpp
```
* (4) 单击屏幕左侧的绿色三角形，即可开始模拟演示。


# 🃏 纸牌消除游戏 Demo

<div align="center">

![Game Logo](Resources/res/card_general.png)

**基于 Cocos2d-x 3.17.2 开发的经典纸牌消除游戏**

[![Cocos2d-x](https://img.shields.io/badge/Cocos2d--x-3.17.2-orange.svg)](https://cocos2d-x.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows-blue.svg)](https://github.com/YUQI-77-space/Care-Game-Demo)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

[📖 程序设计文档](./程序设计文档.md) • [🎮 游戏规则](#游戏规则) • [🏗️ 架构设计](#架构设计)

</div>

---

## ✨ 功能特性

### 🎯 核心玩法
- **🃏 手牌区翻牌替换** - 点击备用堆叠卡牌替换托盘卡牌
- **🎪 桌面牌匹配消除** - 点击与托盘卡牌面值相差1的桌面卡牌
- **⏪ 智能回退系统** - 支持多步操作回退，完美状态恢复

### 🎨 精美视觉
- **🎭 动态卡牌动画** - 流畅的移动和翻转效果
- **👁️ 智能可见性** - 基于位置关系的卡牌覆盖显示
- **🎨 经典扑克设计** - 红黑配色，清晰的数字和花色标识

### 🏗️ 技术亮点
- **🏛️ MVC架构设计** - 清晰的代码分层和职责分离
- **📊 状态管理机制** - 完整的游戏状态快照和恢复
- **⚙️ 配置化关卡** - JSON配置文件支持灵活关卡设计
- **🔧 扩展性设计** - 易于新增卡牌类型和游戏功能

## 🚀 快速开始

### 📋 系统要求
- **操作系统**: Windows 10/11
- **开发环境**: Visual Studio 2019/2022
- **游戏引擎**: Cocos2d-x 3.17.2
- **构建工具**: CMake 3.6+

### 🛠️ 编译运行

1. **克隆项目**
   ```bash
   git clone https://github.com/YUQI-77-space/Care-Game-Demo.git
   cd Care-Game-Demo
   ```

2. **配置 Cocos2d-x 环境**
   - 确保已安装 Cocos2d-x 3.17.2
   - 配置环境变量或修改 CMakeLists.txt 中的 COCOS2DX_ROOT_PATH

3. **构建项目**
   ```bash
   # 使用 CMake 生成 Visual Studio 项目
   mkdir build
   cd build
   cmake .. -G "Visual Studio 16 2019"

   # 或使用 Visual Studio 直接打开
   start proj.win32/A1.sln
   ```

4. **运行游戏**
   - 在 Visual Studio 中编译并运行
   - 或直接运行 `proj.win32/Debug.win32/A1.exe`

## 🎮 游戏规则

### 🎯 游戏目标
通过匹配消除所有桌面卡牌，完成关卡挑战。

### 🎲 匹配规则
- **基本匹配**: 点击桌面卡牌，当其面值与托盘卡牌相差 **±1** 时可匹配
- **花色无关**: 匹配规则 **不考虑花色**，只关注面值差值
- **可见性限制**: 只有 **正面朝上** 的卡牌才能被点击匹配

### 🎪 操作流程
1. **初始状态**: 游戏开始时，系统自动从备用堆叠抽取一张卡牌作为托盘卡牌
2. **翻牌替换**: 点击备用堆叠中的卡牌，将其替换为新的托盘卡牌
3. **匹配消除**: 点击桌面中符合匹配条件的卡牌，该卡牌会移动到托盘位置
4. **覆盖显示**: 被其他卡牌覆盖的卡牌会自动翻转为背面，不可点击
5. **回退操作**: 可随时点击"回退"按钮撤销上一步操作

### 🏆 胜利条件
当桌面区域 **清空所有卡牌** 时，游戏胜利！

## 🏗️ 架构设计

### 📁 项目结构
```
Classes/
├── configs/           # 配置系统
│   ├── models/        # 配置数据模型
│   └── loaders/       # 配置加载器
├── models/            # 数据模型（运行时状态）
├── views/             # UI视图组件
├── controllers/       # 业务逻辑控制器
├── managers/          # 管理器（状态管理、回退）
├── services/          # 服务层（模型生成）
└── utils/             # 工具类和类型定义

Resources/             # 游戏资源
├── res/              # 卡牌资源
├── levels/           # 关卡配置
└── fonts/            # 字体文件
```

### 🏛️ MVC架构详解

#### Model 层（数据模型）
```cpp
class CardModel {        // 单张卡牌数据和逻辑
class GameModel {        // 游戏整体状态管理
class RoundState {       // 单回合状态快照
```

#### View 层（视图展示）
```cpp
class CardView {         // 单张卡牌视图
class GameView {         // 主游戏视图容器
class StackView {        // 手牌区视图
```

#### Controller 层（业务逻辑）
```cpp
class GameController {           // 主控制器
class StackController {          // 手牌区控制器
class PlayFieldController {      // 桌面区域控制器
```

### 🔄 状态管理机制

#### 回合快照系统
- **完整状态捕获**: 记录每个操作后的完整游戏状态
- **内存优化存储**: 使用堆叠状态而非完整对象复制
- **多步回退支持**: 可回退到任意历史回合状态

#### 动画同步机制
- **状态优先**: 动画播放前先更新数据状态
- **回调同步**: 使用动画完成回调确保视觉与数据一致
- **平滑过渡**: 视觉效果与状态变化完美同步

## 📚 程序设计文档

详细的技术实现文档请查看：[程序设计文档](./程序设计文档.md)

该文档包含：
- 📋 完整的项目功能分析
- 🏗️ 详细的架构设计说明
- 🔧 核心算法实现解析
- 📈 性能优化和扩展方案
- 🎯 新功能开发指南

## 🔧 开发指南

### 新增卡牌类型
1. 扩展 `CardFaceType` 和 `CardSuitType` 枚举
2. 更新匹配逻辑 `CardModel::canMatchWith()`
3. 添加资源配置 `CardResConfig` 类
4. 修改视图显示 `CardView::updateDisplay()`

### 扩展回退功能
1. 实现命令模式接口 `GameCommand`
2. 创建具体命令类 `MoveCardCommand` 等
3. 扩展状态管理器 `GameMemory`
4. 更新控制器逻辑封装操作

### 关卡设计
编辑 `Resources/levels/level_1.json` 文件：
```json
{
    "Playfield": [
        {"CardFace": 4, "CardSuit": 2, "Position": {"x": 260, "y": 1100}}
    ],
    "Stack": [
        {"CardFace": 5, "CardSuit": 2, "Position": {"x": 0, "y": 0}}
    ]
}
```

## 🤝 贡献指南

欢迎提交 Issue 和 Pull Request！

1. Fork 本项目
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 创建 Pull Request

## 📄 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 👨‍💻 开发者

**项目作者**: [YUQI-77-space](https://github.com/YUQI-77-space)

## 🙏 致谢

- [Cocos2d-x](https://cocos2d-x.org/) - 优秀的开源游戏引擎
- [RapidJSON](https://rapidjson.org/) - 高速 JSON 解析库

---

<div align="center">

**🎉 享受纸牌消除游戏的乐趣！**

⭐ 如果这个项目对你有帮助，请给它一个星标！

</div>

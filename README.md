# streetcar
有轨电车综合调度与控制系统

=======V1.0.0=======
完成系统升级示例程序
主要由两部分组成：
1、UploadTool，用于上传相应版本程序到Oracle数据库
2、updateDemo，用于演示具体的客户端程序如何升级软件
说明：使用“config.ini”做程序的配置文件，在其中配置有数据库的连接信息和系统升级后需要执行的入口主程序名字。

演示示例说明如下：
1、确保演示系统的Oracle数据库已安装，Oracle Client instance也已安装（这是Oracle客户端，应用程序跟Oracle DB交互需要介由此工具的库完成）
2、以scott/tiger用户的身份执行resources/sql/目录下得sql脚本，创建用于系统升级的数据表
3、执行UploadTool项目中的可执行程序UploadTool，上传一个zip打包的升级程序（resources/example_zip_files目录下有示例文件可用）
4、执行updateDemo项目中的可执行程序updateDemo，点击“升级”按钮，尝试升级

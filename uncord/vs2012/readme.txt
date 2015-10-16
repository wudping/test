1. boost

http://www.boost.org/

下載後 解壓縮

bootstrap.bat   (windows使用)
b2.exe          (編譯)
b2.exe install  (安裝到C槽底下)

設定include, lib目錄
include    C:\Boost\include\boost-1_56
lib        C:\Boost\lib

以上會受到版本  或是bootstrap.bat階段參數不同而有所改變.  我提供的是預設值
沒改path的話,應該會是類似的路徑.



2. QT5

http://qt-project.org/wiki/Building_Qt_5_from_Git

顯卡問題請參考官網,碰到狀況都只能case by case解決orz

Get Perl from: http://www.activestate.com/activeperl/downloads [activestate.com]
Get Python from: http://www.python.org/download/releases/ [python.org]
Get Ruby from: http://www.rubyinstaller.org/downloads/ [rubyinstaller.org]

先下載上面三個東西

Set the environment variable CL (specifying Visual Studio compiler options) to /MP (On the command line: set CL=/MP)
上面那個設置是讓編譯速度加快,不加也ok

我沒有使用ICU.

目錄切換到 D:\code
git clone git://gitorious.org/qt/qt5.git QT5
這個動作不能用source tree 或 tortoisegit, 一定要在cmd視窗下命令

切換目錄到 D:\code\QT5
git checkout 5.4

perl init-repository -no-webkit    
請參考官網說明,webkit問題很多,我都會關掉

configure -developer-build -opensource -nomake examples -nomake tests
這個版本不需要設定目地目錄,開發者版本會再qtbase裡面
但是也可以install到C槽或者/usr/local/底下

編譯命令為   nmake       (-j 4我沒下)

http://qt-project.org/doc/qt-4.8/install-win.html
環境變數設定方式
不同平台請參考對應的網頁
要留意的是mac底下有時候是.profile,有時候會是.bash_profile



openssl

http://slproweb.com/products/Win32OpenSSL.html
我習慣把安裝路徑設定成   C:\OpenSSL_Win32   留意底線

目前使用的是  Win32 OpenSSL v1.0.1j  

這部分的dll請手動複製到release的資料夾內. 並沒有加到git底下.



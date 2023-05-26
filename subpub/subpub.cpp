#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>

using namespace std;

int main(){
  // シリアルポートオープン（/dev/ttyACM0）
  int fd = open("/dev/arduino_mega2560",O_RDWR | O_NOCTTY | O_NDELAY);
  // シリアルポートオープンエラー処理
  if(fd == -1){
    perror("open");
    return -1;
  }
  
  // シリアルポート設定構造体作成
  struct termios options;
  // 現在のシリアルポート設定取得
  if(tcgetattr(fd,&options) == -1){
    perror("tcgetattr");
    return -1;
  }
  
  // シリアルポート設定変更
  options.c_cflag = CS8 | CLOCAL | CREAD; // データサイズ8ビット、ローカル接続、受信可能
  options.c_iflag = IGNPAR; // パリティエラー無視
  options.c_oflag = 0; // 出力処理なし
  options.c_lflag = 0; // カノニカルモードなし、エコーなし
  options.c_cc[VMIN] = 0; // 受信バッファが空でもブロックしない
  options.c_cc[VTIME] = 10; // 受信タイムアウト時間（1秒）
  
  // シリアルポート設定反映
  if(tcsetattr(fd,TCSANOW,&options) == -1){
    perror("tcsetattr");
    return -1;
  }
  
  // 送信するデータとそのサイズ
  char data[] = "Hello Arduino\n";
  int data_size = sizeof(data);
  
  // データ送信
  if(write(fd,data,data_size) == -1){
    perror("write");
    return -1;
  }
  
  // 受信バッファサイズと受信バッファと受信データサイズと受信待ち時間（1000ミリ秒）
  int buffer_size = 256;
  char buffer[buffer_size];
  int read_size = 0;
  int wait_time = 1000;
  
  // 受信処理
  while(true){
    // 受信バッファと受信データサイズ初期化
    memset(buffer,0,buffer_size);
    read_size = 0;
    // 受信待ち時間分だけ待つ
    usleep(wait_time*1000);
    // データ受信
    read_size = read(fd,buffer,buffer_size);
    // 読み込んだバイト数が正であれば
    if(read_size>0){
      // 受信データ表示
      cout << "Received: " << buffer << endl;
      // ループ終了
      break;
    }
    
  }
  
  // シリアルポートクローズ
  close(fd);
  
}
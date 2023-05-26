// ボーレートを9600 bpsに設定
void setup(){
  Serial.begin(115200);
  Serial.println( "Hello Arduino!" );
}

// ループ処理
void loop(){
  // 受信データがあれば
  if(Serial.available()>0){
    // 受信データを読み込む
    char c = Serial.read();
    // 読み込んだデータを送信する
    Serial.write(c);
    Serial.println(c);
  }
}

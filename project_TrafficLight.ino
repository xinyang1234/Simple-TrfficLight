const int triggerPin = 9;
const int echoPin = 10;

const int redPin = 2;
const int yellowPin = 3;
const int greenPin = 4;

long duration;
int distance;

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  // Inisialisasi awal, lampu merah menyala
  digitalWrite(redPin, HIGH);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);

  Serial.begin(9600);
}

void loop() {
  // Mengirimkan sinyal ultrasonik
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Menerima waktu pantulan ultrasonik
  duration = pulseIn(echoPin, HIGH);

  // Menghitung jarak berdasarkan waktu pantulan
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  // Logika untuk mengontrol lampu lalu lintas
  if (distance >= 12) {
    // Jika jarak lebih dari atau sama dengan 10 cm, lampu hijau menyala
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, HIGH);
  } else if (distance >= 6) {
    // Jika jarak lebih dari atau sama dengan 5 cm, lampu kuning menyala
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(yellowPin, HIGH);
    delay(2000);  // Delay 5 detik pada lampu kuning
    digitalWrite(yellowPin, LOW);
  } else {
    // Jika jarak kurang dari cm, lampu merah menyala
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);
    delay(5000);  // Delay 5 detik pada lampu merah
    digitalWrite(redPin, LOW);
  }

  delay(1000); // Delay 1 detik sebelum mengukur kembali
}


#include <WiFi.h>
#include <WebServer.h>
#include <esp_camera.h>
#include <leafDiseaseDetection1_inferencing.h>
#include "edge-impulse-sdk/dsp/image/image.hpp"

// ==== CAMERA CONFIGURATION: AI Thinker ====
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ==== WIFI CREDENTIALS ====
const char* ssid = "A";
const char* password = "111111111";
const char* deviceName = "ESP32-CAM-LEAF";

// ==== CAMERA SETTINGS ====
static camera_config_t camera_config = {
  .pin_pwdn       = PWDN_GPIO_NUM,
  .pin_reset      = RESET_GPIO_NUM,
  .pin_xclk       = XCLK_GPIO_NUM,
  .pin_sscb_sda   = SIOD_GPIO_NUM,
  .pin_sscb_scl   = SIOC_GPIO_NUM,
  .pin_d7         = Y9_GPIO_NUM,
  .pin_d6         = Y8_GPIO_NUM,
  .pin_d5         = Y7_GPIO_NUM,
  .pin_d4         = Y6_GPIO_NUM,
  .pin_d3         = Y5_GPIO_NUM,
  .pin_d2         = Y4_GPIO_NUM,
  .pin_d1         = Y3_GPIO_NUM,
  .pin_d0         = Y2_GPIO_NUM,
  .pin_vsync      = VSYNC_GPIO_NUM,
  .pin_href       = HREF_GPIO_NUM,
  .pin_pclk       = PCLK_GPIO_NUM,
  .xclk_freq_hz   = 20000000,
  .ledc_timer     = LEDC_TIMER_0,
  .ledc_channel   = LEDC_CHANNEL_0,
  .pixel_format   = PIXFORMAT_JPEG,
  .frame_size     = FRAMESIZE_QQVGA,     // Reduced resolution
  .jpeg_quality   = 20,                  // Increased compression
  .fb_count       = 1,
  .fb_location    = CAMERA_FB_IN_PSRAM,
  .grab_mode      = CAMERA_GRAB_WHEN_EMPTY
};

// ==== VARIABLES ====
WebServer server(80);
uint8_t* snapshot_buf;

#define EI_CAMERA_RAW_FRAME_BUFFER_COLS 160
#define EI_CAMERA_RAW_FRAME_BUFFER_ROWS 120
#define EI_CAMERA_FRAME_BYTE_SIZE 3

// ==== AI Inference Image Capture ====
bool ei_camera_capture(uint32_t img_width, uint32_t img_height, uint8_t* out_buf) {
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) return false;

  bool converted = fmt2rgb888(fb->buf, fb->len, PIXFORMAT_JPEG, out_buf);
  esp_camera_fb_return(fb);

  if (!converted) return false;

  if ((img_width != EI_CAMERA_RAW_FRAME_BUFFER_COLS) || (img_height != EI_CAMERA_RAW_FRAME_BUFFER_ROWS)) {
    ei::image::processing::crop_and_interpolate_rgb888(
      out_buf,
      EI_CAMERA_RAW_FRAME_BUFFER_COLS,
      EI_CAMERA_RAW_FRAME_BUFFER_ROWS,
      out_buf,
      img_width,
      img_height
    );
  }
  return true;
}

// ==== Helper for Edge Impulse Classifier ====
static int ei_camera_get_data(size_t offset, size_t length, float* out_ptr) {
  size_t pixel_ix = offset * 3;
  for (size_t i = 0; i < length; i++) {
    out_ptr[i] = (snapshot_buf[pixel_ix + 2] << 16) + (snapshot_buf[pixel_ix + 1] << 8) + snapshot_buf[pixel_ix];
    pixel_ix += 3;
  }
  return 0;
}

// ==== /DETECT ====
void handleDetect() {
  Serial.printf("📦 Free heap before alloc: %u bytes\n", ESP.getFreeHeap());
  snapshot_buf = (uint8_t*)heap_caps_malloc(EI_CAMERA_RAW_FRAME_BUFFER_COLS * EI_CAMERA_RAW_FRAME_BUFFER_ROWS * EI_CAMERA_FRAME_BYTE_SIZE, MALLOC_CAP_SPIRAM);
  if (!snapshot_buf) {
    server.send(500, "text/plain", "❌ Failed to allocate buffer");
    return;
  }

  if (!ei_camera_capture(EI_CLASSIFIER_INPUT_WIDTH, EI_CLASSIFIER_INPUT_HEIGHT, snapshot_buf)) {
    server.send(500, "text/plain", "❌ Image capture failed");
    free(snapshot_buf);
    return;
  }

  ei::signal_t signal;
  signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
  signal.get_data = &ei_camera_get_data;

  ei_impulse_result_t result = { 0 };
  EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);

  if (res != EI_IMPULSE_OK) {
    server.send(500, "text/plain", "❌ Classifier failed");
    free(snapshot_buf);
    return;
  }

  String status = "Healthy";
  for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
    if (String(ei_classifier_inferencing_categories[i]) == "whiteDot's" && result.classification[i].value > 0.6) {
      status = "whiteDot's";
      break;
    }
  }

  free(snapshot_buf);
  server.send(200, "text/plain", status);
}

// ==== /STREAM ====
void handleStream() {
  WiFiClient client = server.client();
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
  client.print(response);

  while (client.connected()) {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) continue;

    client.printf("--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", fb->len);
    client.write(fb->buf, fb->len);
    client.print("\r\n");

    esp_camera_fb_return(fb);
    delay(100);
  }
}

// ==== /CAPTURE ====
void handleCapture() {
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    server.send(500, "text/plain", "❌ Camera capture failed");
    return;
  }
  server.send_P(200, "image/jpeg", (char*)fb->buf, fb->len);
  esp_camera_fb_return(fb);
}

// ==== SETUP ====
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.printf("🔌 Device: %s\n", deviceName);
  Serial.printf("📦 Heap at boot: %u bytes\n", ESP.getFreeHeap());

  if (esp_camera_init(&camera_config) != ESP_OK) {
    Serial.println("❌ Camera init failed!");
    return;
  }

  WiFi.begin(ssid, password);
  Serial.printf("🌐 Connecting to WiFi: %s\n", ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi connected");
  Serial.print("📶 IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/detect", handleDetect);
  server.on("/stream", handleStream);
  server.on("/capture", handleCapture);
  server.begin();
  Serial.println("🌐 HTTP server started");
}

// ==== LOOP ====
void loop() {
  server.handleClient();
}

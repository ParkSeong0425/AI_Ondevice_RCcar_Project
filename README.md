# AI_Ondevice_RCcar_Project

## :notebook::notebook:프로젝트 디렉토리 구조:notebook::notebook:
```
Core/
 main.c
 │
 ├── 시스템 초기화 (HAL_Init, Clock 설정)
 ├── 주변장치 초기화 (GPIO, DMA, UART)
 ├── UART 수신 콜백 함수 정의 (HAL_UART_RxCpltCallback)
 │
 └── 메인 루프 (while(1))

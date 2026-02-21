# SPI Loopback Test

## 概要

SPI通信の基本構造理解のため、
MOSIとMISOを接続したLoopback構成で送受信検証を行った。

## 実装内容

- SPI.begin()により通信初期化
- SPI_MODE0設定で通信実施
- CS制御によるスレーブ選択
- SPI.transfer()を用いた送受信同時処理（Full-Duplex）
- 送信データと受信データの一致確認

## 学習ポイント

- SPIはクロック同期式通信であること
- CS信号により対象デバイスを選択すること
- SPIは全二重通信（Full-Duplex）であること

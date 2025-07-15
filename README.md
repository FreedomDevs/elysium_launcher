## 翻訳:
- [ロシア](README_RU.md)

# Elysium Launcher

**Elysium Launcher** は、`ReactJS`のフロントエンドと `C++`, `Rust`のバックエンドを持つクロスプラットフォームデスクトップアプリケーションです。

## ⚙️ 依存関係のインストール

```bash
./launch.py install
```

- `npm` の依存関係を NodeJS 経由でインストール
- `Cargo`を使って Rust の依存関係をインストール

## 🛠️ ビルド

```bash
./launch.py build
```

- 古いビルドを削除
- フロントエンドをビルド (`npm run build`)
- `dist/` を `build/` にコピー
- Rust ライブラリをビルド
- ヘッダファイルを生成
- `CMake` 経由で C++ バックエンドをビルド

## 🚀 実行

### プロダクション環境:

```bash
./launch.py run
```

コンパイルされたランチャーを実行: `build/elysium_launcher`

### 開発モード:

```bash
./launch.py dev
```

- `npm run dev` を実行（フロント）
- `USE_BUILTIN_WEBSERVER=OFF` で C++ をビルド
- 開発サーバーに接続されたランチャーを起動

停止は `Ctrl+C`、正常に終了します。

## 🧱 プロジェクト構成

```
.
├── app/                  # npm フロントエンド
├── build/                # ビルドディレクトリ
├── include/              # Rust から生成される C ヘッダ
├── libs/
│   └── el-core-lib/      # Rust ライブラリ
├── src/                  # C++ ソース
├── manage.py             # 管理スクリプト
├── CMakeLists.txt
├── LICENSE
└── README.md
```

## ✅ TODO

- [X] ReactJS をプロジェクトに組み込む
- [ ] Windows 対応 (経由 WebView2)
- [ ] CI/CD パイプライン (GitHub Actions)
- [ ] Minecraft の起動とインストール用ライブラリを作成
- [ ] フロントエンドを作成

#pragma once
#include <vector>
#include <stdexcept>

template<typename  T>
class RingBuffer {
public:
	RingBuffer(size_t size) : buffer(size), maxSize(size), head(0), tail(0), full(false) {}

	// データ追加(最大のとき、データは上書きする)
	void push(const T& item) {
		// TODO 04_01
		// データ登録
		// バッファの書き込み用ヘッダ部(buffer[head])にデータ(item)を保存
		//04_01_01

		// ヘッダを+1後ろへずらす
		//ずらしたヘッダ値がデータ書き込みエリア最大値(maxSize)を越えた場合、ヘッダを0にして循環させる
		//04_01_02

		// バッファ領域が最大まで登録されている場合
		if (full) {
			// 取り出し位置(tail)を+1後ろへずらす。
			// ヘッダ同様に、ずらした位置が最大値を越えた場合は循環するように
			//04_01_03
		}

		// 書き出し位置:headと読み出し位置:tailが同じときはいっぱいなので
		//fullのフラグを立てる(fullをtrueにする)
		//04_01_04


	}

	// データ削除
	bool pop(T& item) {

		// データが空の場合falseを返す
		if (empty()) {
			return false;
		}
		// TODO 04_02
		// 呼び出し位置からデータを取り出す(itemにbuffer[tail]を読み出す)
		//04_02_01

		// 次のデータの為、読み出し位置を+1後ろへずらす(最大値を越えた場合は循環するように)
		//04_02_02

		// データが一つ減ったので、確実にいっぱいではなくなるのでfullフラグをおろす(falseに)
		//04_02_03



		return true;
	}
	// インデックスを指定して参照
	// 登録データが取り出されたときや最大値以上追加されたときは参照位置が変わるので注意
	T& at(size_t index) {
	
		// 空の場合、throw std::out_of_range("Buffer is empty");を実行する
		if (empty()) {
			throw std::out_of_range("Buffer is empty");
		}
		size_t s = size();
		// TODO 04_03
		//indexがsize以上の時は登録されているデータ数を越えているので、throw std::out_of_range("Index out of range");を実行する
		//04_03_01

		size_t actualIndex
		// 取り出し位置:tailからインデックスの値:indexを加算してずらした位置を計算して(最大値maxSizeを越えた場合は循環するように)
		//return buffer.at(ずらした位置)で値を返す(下記return行の書き換え)
		//04_03_02
		return buffer.at(0);
	}

	bool empty() const {
		return (!full && (head == tail));
	}

	bool fullBuffer() const {
		return full;
	}

	size_t size() const {
		size_t size = maxSize;

		if (!full) {
			if (head >= tail) {
				size = head - tail;
			}
			else {
				size = maxSize + head - tail;
			}
		}

		return size;
	}

	size_t capacity() const {
		return maxSize;
	}

private:
	std::vector<T> buffer;	// バッファ保存領域
	size_t maxSize;			// 最大値
	size_t head;			// 次のデータの書き込み先
	size_t tail;			// データの取り出し先
	bool full;				// データがバッファ領域最大まで入っているか
};
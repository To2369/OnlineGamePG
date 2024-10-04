#pragma once
#include <vector>
#include <stdexcept>

template<typename  T>
class RingBuffer {
public:
	RingBuffer(size_t size) : buffer(size), maxSize(size), head(0), tail(0), full(false) {}

	// �f�[�^�ǉ�(�ő�̂Ƃ��A�f�[�^�͏㏑������)
	void push(const T& item) {
		// TODO 04_01
		// �f�[�^�o�^
		// �w�b�_�Ƀf�[�^��ۑ�

		// �w�b�_�����炷(�ő�l���z�����ꍇ�͏z����悤��)


		// �o�b�t�@�̈悪�ő�܂œo�^����Ă���ꍇ
		if (full) {
			// ���o���ʒu�����炷�B
			// ���炵���ʒu���ő�l���z�����ꍇ�͏z����悤��

		}

		// �����o���ʒu�ƌĂяo���ʒu�������Ƃ�full�̃t���O�𗧂Ă�

	}

	// �f�[�^�폜
	bool pop(T& item) {
		// TODO 04_02
		// �f�[�^����̏ꍇfalse��Ԃ�
		if (empty()) {
			return false;
		}

		// �Ăяo���ʒu����f�[�^�����o��

		// �Ăяo���ʒu�����炷(�ő�l���z�����ꍇ�͏z����悤��)

		// full�t���O�����낷

		return true;
	}
	// �C���f�b�N�X���w�肵�ĎQ��
	// �o�^�f�[�^�����o���ꂽ�Ƃ���ő�l�ȏ�ǉ����ꂽ�Ƃ��͎Q�ƈʒu���ς��̂Œ���
	T& at(size_t index) {
		// TODO 04_03
		// ��̏ꍇ�Athrow std::out_of_range("Buffer is empty");�����s����
		if (empty()) {
			throw std::out_of_range("Buffer is empty");
		}
		// �o�^����Ă���f�[�^�����z�����l���w�肳�ꂽ�ꍇ�Athrow std::out_of_range("Index out of range");�����s����


		// ���o���ʒu����C���f�b�N�X�̒l���炵���ʒu���v�Z(�ő�l���z�����ꍇ�͏z����悤��)
		
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
	std::vector<T> buffer;	// �o�b�t�@�ۑ��̈�
	size_t maxSize;			// �ő�l
	size_t head;			// ���̃f�[�^�̏������ݐ�
	size_t tail;			// �f�[�^�̎��o����
	bool full;				// �f�[�^���o�b�t�@�̈�ő�܂œ����Ă��邩
};
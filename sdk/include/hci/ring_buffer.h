
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb tw=80 ff=unix :

/**
 * @file hci/ring-buffer.h
 *
 * 环形缓冲区实现
 */
#ifndef HCI_RING_BUFFER_H_
#define HCI_RING_BUFFER_H_

#include <hci/hci.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace hci {

/**
 * 环形缓冲区 (非线程安全)
 *
 * 缓冲区有两种状态 (示意图中 O 表示有数据 . 表示无数据)
 *
 *   1. 数据连续
 *
 *     [....OOOOOOOOO.....]
 *      ^   ^        ^    ^
 *      b   h        t    e
 *
 *   2. 数据不连续
 *
 *     |OOOO.........OOOOO|
 *      ^   ^        ^    ^
 *      b   t        h    e
 *
 * 其中 b, e, h, t 均为索引值
 *
 *   - b 为固定值 0
 *   - e 为固定值，等于缓冲区大小
 *   - h 为数据头部位置，也是下次的数据读取位置
 *   - t 为数据尾部位置，也是下次的数据写入位置
 *
 * 在实际实现中 t 是个虚拟值，允许超过 e，但不会超过或等于 2*e，其实际值
 * 为 T = t - e，如下图
 *
 *      b   T        h    e
 *      |   |        |    |
 *     [OOOO.........OOOOO]
 *
 *     [OOOO.........OOOOO@@@@..............]
 *      ^   ^        ^    ^   ^             ^
 *      b   T        h    e   t             2*e
 *
 * 因此会有如下关系
 *
 *   * b == 0
 *   * h >= 0 && h < e
 *   * t >= h && t < 2*e
 *   * h == t 时，缓冲区空
 *   * h + e == t 时，缓冲区满
 *   * t > e 时，数据不连续
 *   * t <= e 时，数据连续
 */
class HCI_LOCAL HciRingBuffer {
 private:
  typedef unsigned char uint8_t;

  uint8_t *data_;  // 缓冲区内存指针
  size_t size_;    // 缓冲区大小
  size_t head_;    // 头部位置，也是读取位置
  size_t tail_;    // 尾部位置，也是写入位置
  size_t rewind_;  // 头部位置重绕次数

  static const uint8_t *uint8(const void *p) {
    return reinterpret_cast<const uint8_t *>(p);
  }
  static uint8_t *uint8(void *p) {  //
    return reinterpret_cast<uint8_t *>(p);
  }

 public:
  explicit HciRingBuffer(size_t size)
      : size_(size), head_(0), tail_(0), rewind_(0) {
    data_ = uint8(malloc(size_));
  }
  ~HciRingBuffer() { free(data_); }

  // 头部指针
  const uint8_t *head_ptr() const { return data_ + head_; }

  // 头部指针
  uint8_t *head_ptr() { return data_ + head_; }

  // 是否为空
  bool empty() const { return head_ == tail_; }

  // 是否满
  bool full() const { return head_ + size_ == tail_; }

  // 尾部数据是否越过了缓冲区末尾，此时数据不连续
  bool beyond_end() const { return tail_ > size_; }

  // 头部位置，也是读取位置
  size_t head() const { return head_; }
  // 尾部位置，也是写入位置
  size_t tail() const { return tail_; }
  // 缓冲区大小
  size_t size() const { return size_; }

  // 缓冲数据大小
  size_t data_size() const { return tail_ - head_; }
  // 剩余空间大小
  size_t free_size() const { return size_ - data_size(); }
  // 重绕次数
  size_t rewind_count() const { return rewind_; }

  /**
   * 读取数据
   *
   *   @param [out] data_ptr 用于保存`读取数据地址指针`，可以传 nullptr
   *   @param [out] data     用于接收数据的内存首地址
   *   @param [in]  size     准备读取的数据长度
   *
   *   @return  返回成功读取到的数据长度
   *
   * data_ptr 不为空时，若待读取的数据位于连续区域，则 *data_ptr 中保存一个
   * 缓冲区内部地址，以避免数据拷贝操作；否则 *data_ptr 将保存 data 指针值，
   * 并且数据会拷贝至 data 指向的内存区域。
   */
  size_t read(void **data_ptr, void *data, size_t size) {
    uint8_t *dst = uint8(data);
    // 初始化读取区间 [rb, re)
    size_t rb = head_, re = rb + size;
    if (re > tail_) {
      // 缓冲区中没有足够的数据，读取大小需要调整
      re = tail_, size = re - rb;
    }

    if (re > size_) {  // 读取区间跨越缓冲区尾部，需拷贝两次
      size_t s1 = size_ - rb;       // 前半部分数据在缓冲区尾部
      size_t s2 = re - size_;       // 后半部分数据在缓冲区头部
      memcpy(dst, data_ + rb, s1);  // 拷贝前半部分
      memcpy(dst + s1, data_, s2);  // 拷贝后半部分
      if (data_ptr != nullptr) *data_ptr = dst;
    } else {  // 读取区间是一块连续区域
      if (data_ptr != nullptr) {
        *data_ptr = data_ + rb;
      } else {
        memcpy(dst, data_ + rb, size);
        if (data_ptr != nullptr) *data_ptr = dst;
      }
    }
    // 更新 head_
    head_ += size;
    if (head_ >= size_) {
      // 超过缓冲区尾部时同时减去缓冲区长度，恢复至数据连续状态
      head_ -= size_;
      tail_ -= size_;
      ++rewind_;
    }
    return size;
  }

  /**
   * 写入操作
   *
   *   @param [in] data  写入数据内存地址
   *   @param [in] size  写入数据长度
   *
   *   @return 返回成功写入数据的长度
   */
  size_t write(const void *data, size_t size) {
    const uint8_t *src = uint8(data);
    // 初始化写入区间 [wb, we)
    size_t wb = tail_, we = wb + size;
    if (we > head_ + size_) {
      // 缓冲区剩余空间不足，写入大小需要调整
      we = head_ + size_, size = we - wb;
    }
    if (wb >= size_) {
      wb -= size_;
      we -= size_;
    }

    if (we > size_) {
      // 写入区域跨越缓冲区尾部，需要拷贝两次
      size_t s1 = size_ - wb;       // 前半部分数据在缓冲区尾部
      size_t s2 = we - size_;       // 后半部分数据在缓冲区头部
      memcpy(data_ + wb, src, s1);  // 拷贝前半部分
      memcpy(data_, src + s1, s2);  // 拷贝后半部分
    } else {
      // 写入区域是一块连续区域
      memcpy(data_ + wb, src, size);
    }
    tail_ += size;
    return size;
  }

  /**
   * 移动头部指针，相当于空读取
   */
  size_t move_head(size_t size) {
    if (head_ + size > tail_) {
      size = tail_ - head_;
    }
    head_ += size;
    if (head_ >= size_) {
      head_ -= size_;
      tail_ -= size_;
      ++rewind_;
    }
    return size;
  }

  /**
   * 移动尾部指针，相当于空写入
   */
  size_t move_tail(size_t size) {
    if (tail_ + size > head_ + size_) {
      size = head_ + size_ - tail_;
    }
    tail_ += size;
    return size;
  }

  /**
   * 写入一个字节
   */
  bool push_back(uint8_t b) {
    if (full()) return false;
    size_t i = tail_;
    if (i >= size_) i -= size_;
    data_[i] = b;
    ++tail_;
    return true;
  }

  /**
   * 增加缓冲区空间
   */
  void grow(size_t size) {
    // 分配新内存，
    uint8_t *p = uint8(malloc(size_ + size));
    if (tail_ > size_) {
      // 数据不连续，需要拷贝两次数据，拷贝至新内存头部
      size_t s1 = size_ - head_;
      size_t s2 = tail_ - size_;
      memcpy(p, data_ + head_, s1);
      memcpy(p + s1, data_, s2);
    } else {
      // 数据连续
      memcpy(p, data_ + head_, tail_ - head_);
    }
    free(data_);
    data_ = p;
    size_ += size;
    tail_ -= head_;
    head_ = 0;
    rewind_ = 0;
  }

  /**
   * 写入至头部，特殊用法，头部指针会减去 size，不修改 rewind
   */
  bool write_head(const void *data, size_t size) {
    if (free_size() + size > size_)  // 剩余空间不足时不能操作
      return false;
    const uint8_t *src = uint8(data);
    if (head_ >= size) {
      head_ -= size;
      memcpy(data_ + head_, data, size);
    } else {
      size_t s1 = size - head_, s2 = head_;
      memcpy(data_ + size_ - s1, src, s1);
      memcpy(data_, src + s1, s2);
      head_ = size_ - s1;
      tail_ += size_;
    }
    return true;
  }
};

}  // namespace hci

#endif  // HCI_RING_BUFFER_H_

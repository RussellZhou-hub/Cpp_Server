#pragma once
#include <optional>
#include <vector>
#include <thread>
#include <mutex>
#include <deque>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <map>
#include <unordered_map>

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
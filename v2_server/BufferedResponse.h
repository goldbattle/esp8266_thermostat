/*
 * ESP8266 Thermostat System
 * Copyright (C) 2021 Patrick Geneva
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef _BUFFEREDRESPONSE_H
#define _BUFFEREDRESPONSE_H

#include <Arduino.h>

#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif


class BufferedResponse : public Print {
  private:
    uint8_t * buf;
    size_t buffer_size;
#if defined(ESP8266)
    ESP8266WebServer &server;
#else
    WebServer &server;
#endif
    size_t pos = 0;

  public:
#if defined(ESP8266)
    BufferedResponse(ESP8266WebServer &_server, const char *content_type = "text/html", int code = 200, size_t _buffer_size = 4096)
#else
    BufferedResponse(WebServer &_server, const char *content_type = "text/html", int code = 200, size_t _buffer_size = 4096)
#endif
      : server(_server), buffer_size(_buffer_size) {
      buf = (uint8_t  *) malloc(buffer_size);
      server.setContentLength(CONTENT_LENGTH_UNKNOWN);
      if (content_type != NULL)
        server.sendHeader("Content-Type", content_type, true);
      server.sendHeader("Cache-Control", "no-cache");
      server.send(code);
    }

    ~BufferedResponse() {
      if (pos != 0)
        close();
      delete buf;
    }

    size_t write(const uint8_t *data, size_t len)  {
      const size_t buffer_write_size = buffer_size - 1;
      size_t data_pos = 0;
      while(len) {
        //no more space? -> flush
        if((buffer_write_size - pos) == 0)
          flush();
        //how much can we write?
        size_t copy_len = std::min(buffer_write_size - pos, len);
        memcpy((void *) (buf + pos), (const void *) (data + data_pos), copy_len);
        pos += copy_len;
        data_pos += copy_len;
        len -= copy_len;
      }
    }

    size_t write(uint8_t data) {
      return write(&data, 1);
    }

    template <typename stringType>
    void write(const stringType &str) {
      write(str.c_str(), str.length());
    }

    using Print::write;

    void flush() {
      //Flush buffer
      if (pos > 0) {
        buf[pos] = 0;
        server.sendContent(
            (const char *) &buf[0]); //Actually this is a conversion to String - not sure if this is a good idea?
        pos = 0;
      }
    }

    void close() {
      //Flush buffer
      flush();
      //End connection
      server.sendContent("");
    }
};


#endif

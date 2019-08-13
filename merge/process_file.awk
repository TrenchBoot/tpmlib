#!/usr/bin/awk -f

BEGIN {
          in_copyright = 0;
          C = 0;
      }
      {
          if (skip > 0)
              skip = skip - 1;
          if (NR == 1) {
              comment_word = $1;
              if ($1 == "/*") {
                  comment_word = "*";
                  C = 1;
                  in_copyright = 1;
              }
              next;
          }
          if (in_copyright == 1) {
              if ($1 != comment_word)
              {
                  in_copyright = 0;
                  if (C == 1) {
                      if ($1 ~ /\**\//)
                          skip = 1;
                      else
                          exit 1;
                  }
              }
          }
          if (in_copyright == 0) {
              if ($1 ~ /^#include/)
                  skip = 1;
              if (header == 1) {
                  if ($1 ~ /^#ifndef/)
                      skip = 2;
                  if ($1 ~ /^#endif/)
                      skip = 1;
              }
              if ($1 == "static" && $2 == "u8" && $3 == "locality")
                  skip = 1;
              #if ($1 == "static" && $2 == "struct" && $3 == "tpmbuff")
              #    print "__attribute__ ((__section__ (\".data\")))"
          }
          if (in_copyright == 0 && skip == 0)
              print;
      }
END   { }

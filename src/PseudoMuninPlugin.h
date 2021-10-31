// 
// Copyright (c) 2019 smdn (https://smdn.jp/)
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef PseudoMuninPlugin_h
#define PseudoMuninPlugin_h

#include <Arduino.h>
#include <functional> // std::function

struct PseudoMuninPluginField {
  using FetchFunction = std::function<void(PseudoMuninPluginField*)>;

  PseudoMuninPluginField(
    const String& name,
    const FetchFunction fetch = {},
    const double initialValue = 0.0
  ) :
    name(name),
    value(initialValue),
    isValueUnknown(false),
    fetch(fetch)
  {};

  const String name;
  double value;
  bool isValueUnknown;
  const FetchFunction fetch;
};

struct PseudoMuninPlugin {
  PseudoMuninPlugin(
    const String& name,
    const String& config,
    PseudoMuninPluginField* const fields,
    const size_t numberOfFields
  ) :
    name(name),
    config(replaceNewLineChars(config)),
    fieldList(fields),
    numberOfFields(numberOfFields)
  {};

  static String replaceNewLineChars(const String& str);

  const String name;
  const String config;
  PseudoMuninPluginField* const fieldList;
  const size_t numberOfFields;
};

#endif // PseudoMuninPlugin_h
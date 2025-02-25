// hello.cc
#include <node.h>
#include "uv.h"

namespace getHandleAddress {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Number;
using v8::Value;

uv_handle_t* getTcpHandle(void *handleWrap) {
    volatile char *memory = (volatile char *) handleWrap;
    for (volatile uv_handle_t *tcpHandle = (volatile uv_handle_t *) memory; tcpHandle->type != UV_TCP
         || tcpHandle->data != handleWrap || tcpHandle->loop != uv_default_loop(); tcpHandle = (volatile uv_handle_t *) memory) {
        memory++;
    }
    return (uv_handle_t *) memory;
}
void getAddress(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();  
  void *handleWrap = args[0]->ToObject(isolate)->GetAlignedPointerFromInternalField(0);  
  uv_handle_t* handle = getTcpHandle(handleWrap);  
  uv_tcp_t* tcpHandle = (uv_tcp_t*)handle;
  args.GetReturnValue().Set(Number::New(isolate, tcpHandle->socket));
}

void init(Local<Object> exports) {  
  NODE_SET_METHOD(exports, "getAddress", getAddress);
}
NODE_MODULE(addon, init)
}

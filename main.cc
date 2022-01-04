#include "JSEngine.h"
#include "stdio.h"

using namespace std;

char* ResolveModule(const char* identifer, int32_t jsEnvIdx)
{
    printf("ResolveModule:%s\n", identifer);
    if (strcmp(identifer, "main") == 0) 
    {
        return "export delete;";
    }
    else if (strcmp(identifer, "lib.mjs") == 0) {
        return "const a = 'Hello World'; export { a };";
    }
}

void LogCallback(v8::Isolate* Isolate, const v8::FunctionCallbackInfo<v8::Value>& Info, void* Self, int ParamLen, int64_t UserData)
{
    // printf("LogCallbacked\n");
    if (Info[0]->IsString()) {
        v8::String::Utf8Value value(Isolate, Info[0]);
        std::string valueString(*value, value.length());

        printf("%s\n", valueString.c_str());
    }
}

int main(int argc, char** argv)
{
    puerts::JSEngine engine(nullptr, nullptr);
    engine.ModuleResolver = ResolveModule;
    // engine.SetGlobalFunction("log", LogCallback, 0);
    printf("start execute\n");
    engine.ExecuteModule("lib.mjs");

    // engine.Eval("var obj = {}; obj.func();", "");
    v8::Isolate::Scope IsolateScope(engine.MainIsolate);
    v8::HandleScope HandleScope(engine.MainIsolate);
    v8::Local<v8::Context> Context = engine.ResultInfo.Context.Get(engine.MainIsolate);
    v8::Context::Scope ContextScope(Context);

    printf("LastExceptionInfo:%s\n", engine.LastExceptionInfo.c_str());
    if (!engine.ResultInfo.Result.IsEmpty()) {
        printf("ResultType:%d\n", puerts::FV8Utils::GetType(
            Context, 
            *engine.ResultInfo.Result.Get(engine.MainIsolate)
        ));
    }
    // engine.CreateInspector(9222);

    // while (true)
    // {
    //     engine.LogicTick();
    //     engine.InspectorTick();
    // }
}
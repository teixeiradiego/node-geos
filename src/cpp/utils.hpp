#define NODE_GEOS_UNARY_PREDICATE(cppclass, cppmethod, geosmethod)\
\
	typedef struct {\
		cppclass *geom;\
		Nan::Persistent<Function> cb;\
		bool result;\
	} geosmethod##_baton_t;\
\
	void cppclass::cppmethod##Async(uv_work_t *req) {\
		geosmethod##_baton_t *closure = static_cast<geosmethod##_baton_t *>(req->data);\
		closure->result = closure->geom->_instance->geosmethod();\
	}\
\
	void cppclass::cppmethod##AsyncComplete(uv_work_t *req, int status) {\
\
		Isolate* isolate = Isolate::GetCurrent();\
		HandleScope scope(isolate);\
\
		assert(status == 0);\
		geosmethod##_baton_t *closure = static_cast<geosmethod##_baton_t *>(req->data);\
		Local<Value> argv[2] = {\
			Null(isolate),\
			closure->result ? True(isolate) : False(isolate)\
		};\
		TryCatch tryCatch(isolate);\
		Local<Function> local_callback = Local<Function>::New(isolate, closure->cb);\
		local_callback->Call(isolate->GetCurrentContext()->Global(), 2, argv);\
\
		if(tryCatch.HasCaught()) {\
			FatalException(isolate, tryCatch);\
		}\
\
		closure->cb.Reset();\
		closure->geom->Unref();\
\
		delete closure;\
		delete req;\
	}\
\
	NAN_METHOD(cppclass::cppmethod) {\
\
		cppclass *geom = Nan::ObjectWrap::Unwrap<cppclass>(info.Holder());\
\
		if (info.Length() == 1) {\
\
			geosmethod##_baton_t *closure = new geosmethod##_baton_t();\
			closure->geom = geom;\
			closure->cb.Reset(Local<Function>::Cast(info[0]));\
			uv_work_t *req = new uv_work_t;\
			req->data = closure;\
			uv_queue_work(uv_default_loop(), req, cppmethod##Async, cppmethod##AsyncComplete);\
			geom->Ref();\
			info.GetReturnValue().Set(Nan::Undefined());\
\
		} else {\
\
			try {\
\
				info.GetReturnValue().Set(\
					geom->_instance->geosmethod() ? Nan::True() : Nan::False()\
				);\
\
			} catch(geos::util::GEOSException exception) {\
				Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());\
			}\
\
		}\
\
	}


#define NODE_GEOS_BINARY_PREDICATE(cppclass, cppclass2, cppmethod, geosmethod)\
\
	typedef struct {\
		cppclass *geom;\
		cppclass2 *geom2;\
		Nan::Persistent<Function> cb;\
		bool result;\
	} geosmethod##_baton_t;\
\
	void cppclass::cppmethod##Async(uv_work_t *req) {\
		geosmethod##_baton_t *closure = static_cast<geosmethod##_baton_t *>(req->data);\
		closure->result = closure->geom->_instance->geosmethod(closure->geom2->_instance);\
	}\
\
	void cppclass::cppmethod##AsyncComplete(uv_work_t *req, int status) {\
\
		Isolate* isolate = Isolate::GetCurrent();\
		HandleScope scope(isolate);\
\
		assert(status == 0);\
		geosmethod##_baton_t *closure = static_cast<geosmethod##_baton_t *>(req->data);\
		Local<Value> argv[2] = {\
			Null(isolate), closure->result ? True(isolate) : False(isolate)\
		};\
		TryCatch tryCatch(isolate);\
		Local<Function> local_callback = Local<Function>::New(isolate, closure->cb);\
		local_callback->Call(isolate->GetCurrentContext()->Global(), 2, argv);\
\
		if(tryCatch.HasCaught()) {\
			FatalException(isolate, tryCatch);\
		}\
\
		closure->cb.Reset();\
		closure->geom->Unref();\
		closure->geom2->_unref();\
\
		delete closure;\
		delete req;\
\
	}\
\
	NAN_METHOD(cppclass::cppmethod) {\
\
		cppclass *geom = Nan::ObjectWrap::Unwrap<cppclass>(info.Holder());\
		cppclass2 *geom2 = Nan::ObjectWrap::Unwrap<cppclass2>(info[0]->ToObject());\
\
		Local<Function> f = Local<Function>::Cast(info[1]);\
\
		if (info.Length() == 2) {\
\
			geosmethod##_baton_t *closure = new geosmethod##_baton_t();\
			closure->geom = geom;\
			closure->geom2 = geom2;\
			closure->cb.Reset(Nan::Persistent<Function>(f));\
\
			uv_work_t *req = new uv_work_t;\
			req->data = closure;\
\
			uv_queue_work(uv_default_loop(), req, cppmethod##Async, cppmethod##AsyncComplete);\
			geom->Ref();\
			geom2->_ref();\
\
			info.GetReturnValue().Set(Nan::Undefined());\
\
		} else {\
\
			try {\
\
				info.GetReturnValue().Set(\
					geom->_instance->geosmethod(geom2->_instance) ? Nan::True() : Nan::False()\
				);\
\
			} catch(geos::util::GEOSException exception) {\
				Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());\
			}\
\
		}\
\
	}

#define NODE_GEOS_UNARY_TOPOLOGIC_FUNCTION(cppnamespace, cppclass, cppmethod, geosmethod)\
\
	NAN_METHOD(cppclass::cppmethod) {\
\
		cppclass *geom = Nan::ObjectWrap::Unwrap<cppclass>(info.Holder());\
\
		try {\
\
			cppnamespace::cppclass* result = geom->_instance->geosmethod();\
			info.GetReturnValue().Set(cppclass::New(result));\
\
		} catch(geos::util::GEOSException exception) {\
				Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());\
		}\
\
	}

#define NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(cppnamespace, cppclass, cppmethod, geosmethod)\
\
	NAN_METHOD(cppclass::cppmethod) {\
\
		cppclass *geom = Nan::ObjectWrap::Unwrap<cppclass>(info.Holder());\
		cppclass *geom2 = Nan::ObjectWrap::Unwrap<cppclass>(info[0]->ToObject());\
\
		try {\
\
			cppnamespace::cppclass* result = geom->_instance->geosmethod(geom2->_instance);\
			info.GetReturnValue().Set(cppclass::New(result));\
\
		} catch(geos::util::GEOSException exception) {\
				Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());\
		}\
\
	}

#define NODE_GEOS_DOUBLE_GETTER(cppclass, cppmethod, geosmethod)\
\
	NAN_METHOD(cppclass::cppmethod) {\
\
		cppclass *geom = Nan::ObjectWrap::Unwrap<cppclass>(info.Holder());\
\
		try {\
			info.GetReturnValue().Set(geom->_instance->geosmethod());\
		} catch(geos::util::GEOSException exception) {\
				Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());\
		}\
\
	}

#define NODE_GEOS_V8_FUNCTION(cppmethod)\
	static NAN_METHOD(cppmethod);\
	static void cppmethod##Async(uv_work_t *req);\
	static void cppmethod##AsyncComplete(uv_work_t *req, int status);

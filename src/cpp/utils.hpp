#define NODE_GEOS_UNARY_PREDICATE(cppclass, cppmethod, geosmethod)\
\
	class cppmethod##Worker : public Nan::AsyncWorker {\
		public:\
			cppmethod##Worker(Nan::Persistent<v8::Promise::Resolver> *persistent, geos::geom::cppclass *geom) : Nan::AsyncWorker(NULL) {\
				_persistent = persistent;\
				_geom = geom;\
			}\
			~cppmethod##Worker() {};\
			virtual void Execute() {\
				try {\
					_result = _geom->geosmethod();\
				} catch(const geos::util::GEOSException& exception) {\
					this->SetErrorMessage(exception.what());\
				}\
			}\
			virtual void HandleOKCallback() {\
				Nan::HandleScope scope;\
				auto resolver = Nan::New(*_persistent);\
				resolver->Resolve(_result ? Nan::True() : Nan::False());\
			}\
			virtual void HandleErrorCallback() {\
				Nan::HandleScope scope;\
				auto resolver = Nan::New(*_persistent);\
				resolver->Reject(Nan::New(this->ErrorMessage()).ToLocalChecked());\
			}\
		private:\
			Nan::Persistent<v8::Promise::Resolver> *_persistent;\
			geos::geom::cppclass *_geom;\
			bool _result;\
	};\
\
	NAN_METHOD(cppclass::cppmethod##Async) {\
		cppclass *geom = Nan::ObjectWrap::Unwrap<cppclass>(info.Holder());\
		Local<v8::Promise::Resolver> resolver = Promise::Resolver::New(info.GetIsolate());\
		Nan::Persistent<Promise::Resolver> *persistent = new Nan::Persistent<Promise::Resolver>(resolver);\
		cppmethod##Worker *aw = new cppmethod##Worker(persistent, geom->_instance);\
		Nan::AsyncQueueWorker(aw);\
		info.GetReturnValue().Set(resolver->GetPromise());\
	}\
\
	NAN_METHOD(cppclass::cppmethod) {\
		cppclass *geom = Nan::ObjectWrap::Unwrap<cppclass>(info.Holder());\
		try {\
			info.GetReturnValue().Set(\
				geom->_instance->geosmethod() ? Nan::True() : Nan::False()\
			);\
		} catch(const geos::util::GEOSException& exception) {\
			Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());\
		}\
	}


#define NODE_GEOS_BINARY_PREDICATE(namespace, cppclass, namespace2, cppclass2, cppmethod, geosmethod)\
\
	class cppmethod##Worker : public Nan::AsyncWorker {\
		public:\
			cppmethod##Worker(Nan::Persistent<v8::Promise::Resolver> *persistent, const namespace::cppclass *geom, const namespace2::cppclass2 *geom2) : Nan::AsyncWorker(NULL) {\
				_persistent = persistent;\
				_geom = geom;\
				_geom2 = geom2;\
			}\
			~cppmethod##Worker() {};\
			virtual void Execute() {\
				try {\
					_result = _geom->geosmethod(_geom2);\
				} catch(const geos::util::GEOSException& exception) {\
					this->SetErrorMessage(exception.what());\
				}\
			}\
			virtual void HandleOKCallback() {\
				Nan::HandleScope scope;\
				auto resolver = Nan::New(*_persistent);\
				resolver->Resolve(_result ? Nan::True() : Nan::False());\
			}\
			virtual void HandleErrorCallback() {\
				Nan::HandleScope scope;\
				auto resolver = Nan::New(*_persistent);\
				resolver->Reject(Nan::New(this->ErrorMessage()).ToLocalChecked());\
			}\
		private:\
			Nan::Persistent<v8::Promise::Resolver> *_persistent;\
			const namespace::cppclass *_geom;\
			const namespace2::cppclass2 *_geom2;\
			bool _result;\
	};\
\
	NAN_METHOD(cppclass::cppmethod##Async) {\
		cppclass *geom = Nan::ObjectWrap::Unwrap<cppclass>(info.Holder());\
		cppclass2 *geom2 = Nan::ObjectWrap::Unwrap<cppclass2>(info[0]->ToObject());\
		Local<v8::Promise::Resolver> resolver = Promise::Resolver::New(info.GetIsolate());\
		Nan::Persistent<Promise::Resolver> *persistent = new Nan::Persistent<Promise::Resolver>(resolver);\
		cppmethod##Worker *aw = new cppmethod##Worker(persistent, geom->_instance, geom2->_instance);\
		Nan::AsyncQueueWorker(aw);\
		info.GetReturnValue().Set(resolver->GetPromise());\
	}\
\
	NAN_METHOD(cppclass::cppmethod) {\
		cppclass *geom = Nan::ObjectWrap::Unwrap<cppclass>(info.Holder());\
		cppclass2 *geom2 = Nan::ObjectWrap::Unwrap<cppclass2>(info[0]->ToObject());\
		try {\
			info.GetReturnValue().Set(\
				geom->_instance->geosmethod(geom2->_instance) ? Nan::True() : Nan::False()\
			);\
		} catch(const geos::util::GEOSException& exception) {\
			Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());\
		}\
	}

#define NODE_GEOS_UNARY_TOPOLOGIC_FUNCTION(cppnamespace, cppclass, cppmethod, geosmethod)\
\
	NAN_METHOD(cppclass::cppmethod) {\
		cppclass *geom = Nan::ObjectWrap::Unwrap<cppclass>(info.Holder());\
		try {\
			cppnamespace::cppclass* result = geom->_instance->geosmethod();\
			info.GetReturnValue().Set(cppclass::New(result));\
		} catch(const geos::util::GEOSException& exception) {\
				Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());\
		}\
	}

#define NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(cppnamespace, cppclass, cppmethod, geosmethod)\
\
	NAN_METHOD(cppclass::cppmethod) {\
		cppclass *geom = Nan::ObjectWrap::Unwrap<cppclass>(info.Holder());\
		cppclass *geom2 = Nan::ObjectWrap::Unwrap<cppclass>(info[0]->ToObject());\
		try {\
			cppnamespace::cppclass* result = geom->_instance->geosmethod(geom2->_instance);\
			info.GetReturnValue().Set(cppclass::New(result));\
		} catch(const geos::util::GEOSException& exception) {\
				Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());\
		}\
	}

#define NODE_GEOS_DOUBLE_GETTER(cppclass, cppmethod, geosmethod)\
\
	NAN_METHOD(cppclass::cppmethod) {\
		cppclass *geom = Nan::ObjectWrap::Unwrap<cppclass>(info.Holder());\
		try {\
			info.GetReturnValue().Set(geom->_instance->geosmethod());\
		} catch(const geos::util::GEOSException& exception) {\
				Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());\
		}\
	}

#define NODE_GEOS_V8_FUNCTION(cppmethod)\
	static NAN_METHOD(cppmethod);\
	static NAN_METHOD(cppmethod##Async);

#pragma once

template<typename T,typename O>
O array_to_octave_matrix( array<T,2>^ matrix){
	/*Console::WriteLine("CLR Array:");

	for(int i=0;i<matrix->GetLength(0);i++){
		for(int j=0;j<matrix->GetLength(1);j++){
			Console::WriteLine(" {0}",matrix[i,j]);
		}}*/

	O marray(dim_vector(matrix->GetLength(0),matrix->GetLength(1)));
	pin_ptr<T> ptr=&matrix[0,0];
	memcpy(marray.fortran_vec(),ptr,marray.byte_size());

	/*std::ostringstream s;

	s<<marray;
	Console::WriteLine("Octave array");
	Console::WriteLine(msclr::interop::marshal_as<String^>(s.str()));*/

	return marray;
}
 octave_value object_to_octave_value(Object^ obj){
	 auto t=obj->GetType();

	 if(t->IsArray){
		 Console::WriteLine("Array");
     if(t->GetArrayRank()!=2){
		 throw std::bad_typeid();
	 }

     auto st=t->GetElementType();

	 if(st==System::Double::typeid){
        return array_to_octave_matrix<System::Double,Matrix>(static_cast<array<System::Double,2>^>(obj));
	 }else if(st==System::Single::typeid){
		 return array_to_octave_matrix<System::Single,FloatMatrix>(static_cast<array<System::Single,2>^>(obj));
	 }else if(st==System::Int64::typeid){
		 return array_to_octave_matrix<System::Int64,int64NDArray>(static_cast<array<System::Int64,2>^>(obj));
	 }else if(st==System::Int32::typeid){
		 return array_to_octave_matrix<System::Int32,int32NDArray>(static_cast<array<System::Int32,2>^>(obj));
	 }else if(st==System::Int16::typeid){
		 return array_to_octave_matrix<System::Int16,int64NDArray>(static_cast<array<System::Int16,2>^>(obj));
	 }else if(st==System::SByte::typeid){
		 return array_to_octave_matrix<System::SByte,int8NDArray>(static_cast<array<System::SByte,2>^>(obj));
	 }else if(st==System::UInt64::typeid){
		 return array_to_octave_matrix<System::UInt64,uint64NDArray>(static_cast<array<System::UInt64,2>^>(obj));
	 }else if(st==System::UInt32::typeid){
		 return array_to_octave_matrix<System::UInt32,uint32NDArray>(static_cast<array<System::UInt32,2>^>(obj));
	 }else if(st==System::UInt16::typeid){
		 return array_to_octave_matrix<System::UInt16,uint64NDArray>(static_cast<array<System::UInt16,2>^>(obj));
	 }else if(st==System::Byte::typeid){
		 return array_to_octave_matrix<System::Byte,uint8NDArray>(static_cast<array<System::Byte,2>^>(obj));
	 }else if(st==System::Boolean::typeid){
		 return array_to_octave_matrix<System::Boolean,boolMatrix>(static_cast<array<System::Boolean,2>^>(obj));
	 }else if(st==System::Object::typeid){
		 auto vars= static_cast<array<System::Object^,2>^>(obj);
         auto cell=Cell(dim_vector(vars->GetLength(0),vars->GetLength(1)));

		 for(int i=0;i<cell.dim1();i++)
			 for(int j=0;j<cell.dim2();j++){
				 cell(i,j)=object_to_octave_value(vars[i,j]);
			 }

		 return cell;
	 }else{
		 throw std::bad_typeid();
	 }
	 } else if(t->IsPrimitive){
		 if(t==System::Double::typeid){
			 return static_cast<double>(obj);
		 }else if(t==System::Single::typeid){
			 return static_cast<float>(obj);
		 }else if (t==System::Int64::typeid){
			 return static_cast<int64_t >(obj);
		 }else if (t==System::Int32::typeid){
			 return static_cast<int32_t >(obj);
		 }else if (t==System::Int16::typeid){
			 return static_cast<int16_t >(obj);
		 }else if (t==System::SByte::typeid){
			 return static_cast<int8_t >(obj);
		 }else if (t==System::UInt64::typeid){
			 return static_cast<uint64_t >(obj);
		 }else if (t==System::UInt32::typeid){
			 return static_cast<uint32_t >(obj);
		 }else if (t==System::UInt16::typeid){
			 return static_cast<uint16_t >(obj);
		 }else if (t==System::Byte::typeid){
			 return static_cast<uint8_t >(obj);
		 }else if(t==System::Boolean::typeid){
			 return static_cast<bool>(obj);
		 }else{
			 throw std::bad_typeid();
		 }
	 }else if(t==System::String::typeid){
		 return msclr::interop::marshal_as<std::string>(static_cast<System::String^>(obj));
	 }else{
		 throw std::bad_typeid();
	 }
}

octave_value_list object_to_octave_value_list(array<Object^>^ args){
	octave_value_list oargs(args->Length);

	

	for (int i=0;i<args->Length;i++)
	{
		Console::WriteLine("TryConvert:{0}",args[i]->GetType());

		oargs(i)=object_to_octave_value(args[i]);
	}

	return oargs;
}

template<typename T,typename O>
array<T,2>^  octave_matrix_to_array( O& marray){
	array<T,2>^ matrix = gcnew array<T,2>(marray.dim1(),marray.dim2());

	pin_ptr<T> ptr=&matrix[0,0];
	memcpy(ptr,marray.fortran_vec(),marray.byte_size());

	return matrix;
}

Object^ octave_value_to_object(const octave_value& value){
	if(value.is_matrix_type()){
		if(value.is_double_type()){
			return octave_matrix_to_array<System::Double,Matrix>(value.matrix_value());
		}else if(value.is_float_type()){
			return octave_matrix_to_array<System::Single,FloatMatrix>(value.float_matrix_value());
		}else if(value.is_int64_type()){
			return octave_matrix_to_array<System::Int64,int64NDArray>(value.int64_array_value());
		}else if(value.is_int32_type()){
			return octave_matrix_to_array<System::Int32,int32NDArray>(value.int32_array_value());
		}else if(value.is_int16_type()){
			return octave_matrix_to_array<System::Int16,int16NDArray>(value.int16_array_value());
		}else if(value.is_int8_type()){
			return octave_matrix_to_array<System::SByte,int8NDArray>(value.int8_array_value());
		}else if(value.is_uint64_type()){
			return octave_matrix_to_array<System::UInt64,uint64NDArray>(value.uint64_array_value());
		}else if(value.is_uint32_type()){
			return octave_matrix_to_array<System::UInt32,uint32NDArray>(value.uint32_array_value());
		}else if(value.is_uint16_type()){
			return octave_matrix_to_array<System::UInt16,uint16NDArray>(value.uint16_array_value());
		}else if(value.is_uint8_type()){
			return octave_matrix_to_array<System::Byte,uint8NDArray>(value.uint8_array_value());
		}else if(value.is_bool_type()){
			return octave_matrix_to_array<System::Boolean,boolMatrix>(value.bool_matrix_value());
		}else if(value.is_char_matrix()){
			return octave_matrix_to_array<System::Byte,charMatrix>(value.char_matrix_value());
		}else {
			throw std::bad_typeid();
		}
	}else if(value.is_scalar_type()){
		if(value.is_double_type()){
			return static_cast<System::Double>(value.double_value());
		}else if(value.is_float_type()){
			return static_cast<System::Single>(value.float_value());
		}else if(value.is_int64_type()){
			return static_cast<System::Int64>(value.int64_scalar_value());
		}else if(value.is_int32_type()){
			return static_cast<System::Int32>(value.int32_scalar_value());
		}else if(value.is_int16_type()){
			return static_cast<System::Int16>(value.int16_scalar_value());
		}else if(value.is_int8_type()){
			return static_cast<System::SByte>(value.int8_scalar_value());
		}else if(value.is_uint64_type()){
			return static_cast<System::UInt64>(value.uint64_scalar_value());
		}else if(value.is_uint32_type()){
			return static_cast<System::UInt32>(value.uint32_scalar_value());
		}else if(value.is_uint16_type()){
			return static_cast<System::UInt16>(value.uint16_scalar_value());
		}else if(value.is_uint8_type()){
			return static_cast<System::Byte>(value.uint8_scalar_value());
		}else if(value.is_bool_type()){
			return static_cast<System::Boolean>(value.bool_value());
		}else {
			throw std::bad_typeid();
		}
	   }else if(value.is_cell()){
		auto cell=value.cell_value();

		auto ret=gcnew array<Object^,2>(cell.dim1(),cell.dim2());

		for(int i=0;i<cell.dim1();i++)
			for(int j=0;j<cell.dim2();j++){
				ret[i,j]=octave_value_to_object(cell(i,j));
			}
			return ret;
	}else if(value.is_string()){
		return msclr::interop::marshal_as<String^>(value.string_value());
	}else{
		throw std::bad_typeid();
	}
}

Object^  octave_list_to_object(octave_value_list& args){
	array<Object^>^ ret= gcnew array<Object^>(args.length());

	for (int i=0;i<args.length();i++)
	{
		ret[i]=octave_value_to_object(args(i));
	}

	if(ret->Length==1){
		return ret[0];
	}
	else
	{
		return ret;
	}
}
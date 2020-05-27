class compiler {
	
	virtual void copyArgument (nameTable* variables, tree_st index)
	{
		const byte com_push_arguments_0[] =
        {
        0x55,                           // push rbp
        0x48, 0xBB,                     // mov  rbx, imm64

        0x00, 0x00, 0x00, 0x00,         // imm64
        0x00, 0x00, 0x00, 0x00,         //

        0x58,                           // pop  rax
        0x48, 0x29, 0xD8,               // sub  rax, rbx
        0xFF, 0x30                      // push QWORD ptr [rax]
        };
				...
	};
	
};


class optCompiler: compiler {
	virtual void copyArgument (nameTable* variables, tree_st index)
	{
		const byte com_push_arguments_1[] =
        {
        0xFF, 0xB5, 0x00, 0x00, 0x00, 0x00      // push qword ptr [rbp + 0]
        };

				...
	};
};


compiler c;
optcompiler oc;
if (strcmp (argv[1], "-O0") == 0) { work(&c); }
if (strcmp (argv[1], "-O1") == 0) { work(&oc); }

void work (compiler* comp) {
	...
	comp->copyArgument(...);
	...
}
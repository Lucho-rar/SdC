extern int asm_main(float);

int add_one(float n) {
    int res = asm_main(n);
    return res;
}

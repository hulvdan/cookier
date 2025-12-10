define compile_shader
	@echo [$(1)]
	$(SILENT) $(SHADERC) --type $(2) --platform linux   -p 120        -f $(1) -o "$(SHADER_TMP)" --bin2c $(basename $(1))_glsl
	@cat "$(SHADER_TMP)" > $(basename $(1)).bin.h
	-$(SILENT) $(SHADERC) --type $(2) --platform android -p 100_es     -f $(1) -o "$(SHADER_TMP)" --bin2c $(basename $(1))_essl
	-@cat "$(SHADER_TMP)" >> $(basename $(1)).bin.h
	-$(SILENT) $(SHADERC) --type $(2) --platform linux   -p spirv      -f $(1) -o "$(SHADER_TMP)" --bin2c $(basename $(1))_spv
	-@cat "$(SHADER_TMP)" >> $(basename $(1)).bin.h
	-$(SILENT) $(SHADERC) --type $(2) --platform windows -p s_4_0 -O 3 -f $(1) -o "$(SHADER_TMP)" --bin2c $(basename $(1))_dx11
	-@cat "$(SHADER_TMP)" >> $(basename $(1)).bin.h
	-$(SILENT) $(SHADERC) --type $(2) --platform ios     -p metal -O 3 -f $(1) -o "$(SHADER_TMP)" --bin2c $(basename $(1))_mtl
	-@cat "$(SHADER_TMP)" >> $(basename $(1)).bin.h
	-@printf "extern const uint8_t* $(basename $(1))_pssl;\n" | tr -d '\015' >> $(basename $(1)).bin.h
	-@printf "extern const uint32_t $(basename $(1))_pssl_size;\n" | tr -d '\015' >> $(basename $(1)).bin.h
endef

.PHONY: compile
compile:
	$(call compile_shader,$(MY_FILE),$(TYPE))

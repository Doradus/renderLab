#pragma once
#include <string>
#include "MathUtils.h"
#include "RenderingInterfaceResources.h"
#include "Material.h"
#include "MaterialCompiler.h"
#include "IMaterialNode.h"


class ScalarNode : public MaterialNode {
public:
	ScalarNode(Material* material);
	~ScalarNode();

	virtual void		GetValue(DirectX::XMFLOAT4* outValue) const override;
	virtual int			Compile(class MaterialCompiler* compiler) override;
	void				SetValues(float r);
	float				R;
};

class Vector3Node : public MaterialNode {
public:
	Vector3Node(Material* material);
	~Vector3Node();

	virtual void		GetValue(DirectX::XMFLOAT4* outValue) const override;
	virtual int			Compile(class MaterialCompiler* compiler) override;

	void				SetValues(float r, float g, float b);
	float				R;
	float				G;
	float				B;
};

// ** textures ** //

class TextureSamplerNode : public MaterialNode {
public:
	TextureSamplerNode(Material* material);
	~TextureSamplerNode();

	virtual void			GetValue(DirectX::XMFLOAT4* outValue) const override;
	virtual int				Compile(class MaterialCompiler* compiler) override;

	TextureRI*				GetTexture() const;
	void					AddTexture(TextureRI* inTexture);

	unsigned int			filterMode;
	float					LodBias;

private:
	unsigned int textureIndex;
	TextureRI* texture;
};
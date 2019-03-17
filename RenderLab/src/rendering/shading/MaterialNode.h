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
	TextureSamplerNode(Material* material, TextureTyes type);
	~TextureSamplerNode();

	virtual void			GetValue(DirectX::XMFLOAT4* outValue) const override;
	virtual int				Compile(class MaterialCompiler* compiler) override;

	TextureRI*				GetTexture() const;
	void					AddTexture(TextureRI* inTexture);

	float					LodBias;
	TextureTyes				textureType;

private:
	unsigned int textureIndex;
	TextureRI* texture;
};

class Multiply : public MaterialNode {
public:
	Multiply(Material* material);
	~Multiply();

	MaterialNode*		a;
	MaterialNode*		b;

	virtual void		GetValue(DirectX::XMFLOAT4* outValue) const override;
	virtual int			Compile(class MaterialCompiler* compiler) override;
};

class Add : public MaterialNode {
public:
	Add(Material* material);
	~Add();

	MaterialNode*		a;
	MaterialNode*		b;

	virtual void		GetValue(DirectX::XMFLOAT4* outValue) const override;
	virtual int			Compile(class MaterialCompiler* compiler) override;
};
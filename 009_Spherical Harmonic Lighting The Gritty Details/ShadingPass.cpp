#include "ShadingPass.h"
#include "Shader.h"
#include "Sponza.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>


CShadingPass::CShadingPass(const std::string& vPassName, int vExcutionOrder) :IRenderPass(vPassName, vExcutionOrder)
{
}

CShadingPass::~CShadingPass()
{
}

//************************************************************************************
//Function:
void CShadingPass::initV()
{

	getCoefs();
	ElayGraphics::Camera::setMainCameraFarPlane(100);
	ElayGraphics::Camera::setMainCameraPos({ -1.57278, 0.244948, 0.367194 });
	ElayGraphics::Camera::setMainCameraFront({ 0.967832, -0.112856, -0.224865 });
	ElayGraphics::Camera::setMainCameraMoveSpeed(0.1);
	m_pShader = std::make_shared<CShader>("Sponza_VS.glsl", "Sponza_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<CSponza>(ElayGraphics::ResourceManager::getGameObjectByName("Sponza"));
	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	for (int i = 0; i < m_Coefs.size(); i++)
	{
		m_pShader->setFloatUniformValue("u_Coef[" + std::to_string(i) + "]", m_Coefs[i].x, m_Coefs[i].y, m_Coefs[i].z);
	}
	m_pSponza->initModel(*m_pShader);
}

//************************************************************************************
//Function:
void CShadingPass::getCoefs()
{
	std::string path = "../Textures/Skybox/coefficients.txt";
	std::ifstream ifs(path);
	if (!ifs)
		throw std::runtime_error("open " + path + " failed");
	int i = 0;
	float r, g, b;
	while (ifs >> r >> g >> b)
	{
		m_Coefs.push_back(glm::vec3(r, g, b));
		i++;
	}
}

//************************************************************************************
//Function:
void CShadingPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_pShader->activeShader();

	m_pSponza->updateModel(*m_pShader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
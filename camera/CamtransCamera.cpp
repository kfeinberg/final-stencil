#include "CamtransCamera.h"

#include <glm/gtc/reciprocal.hpp>

CamtransCamera::CamtransCamera()
{
    setClip(1.0f, 30.0f);
    setHeightAngle(60.0f);
    setAspectRatio(1.0f);
    glm::vec4 eye = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);
    glm::vec4 look = glm::vec4(-2.0f, -2.0f, -2.0f, 0.0f);
    glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    orientLook(eye, look, up);
}

void CamtransCamera::setAspectRatio(float a)
{
    m_aspectRatio = a;
    updateProjectionMatrix();
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    return m_perspectiveTransformation * m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    return m_rotationMatrix * m_translationMatrix;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const {
    return -m_w;
}

glm::vec4 CamtransCamera::getUp() const {
    return m_up;
}

glm::vec4 CamtransCamera::getU() const {
    return m_u;
}

glm::vec4 CamtransCamera::getV() const {
    return m_v;
}

glm::vec4 CamtransCamera::getW() const {
    return m_w;
}

float CamtransCamera::getAspectRatio() const {
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    return m_thetaH;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    m_eye = eye;
    m_up = up;
    m_w = glm::normalize(-look);
    m_v = glm::normalize(m_up - (glm::dot(m_up, m_w) * m_w));
    m_u = glm::vec4(glm::cross(m_v.xyz(), m_w.xyz()), 0.0f);
    updateViewMatrix();
    updateProjectionMatrix();
}

void CamtransCamera::setHeightAngle(float h) {
    m_thetaH = glm::radians(h);

    float height = 2.0f * m_far * glm::tan(m_thetaH / 2.0f);
    float width = m_aspectRatio * height;
    m_thetaW = 2.0f * glm::atan(width / (2.0f * m_far));
    updateProjectionMatrix();
}

void CamtransCamera::translate(const glm::vec4 &v) {
    m_eye += v;
    updateViewMatrix();
}

void CamtransCamera::rotateU(float degrees) {
    float theta = glm::radians(degrees);
    glm::vec4 v = m_w * glm::sin(theta) + m_v * glm::cos(theta);
    glm::vec4 w = m_w * glm::cos(theta) - m_v * glm::sin(theta);
    m_v = v;
    m_w = w;
    updateViewMatrix();
}

void CamtransCamera::rotateV(float degrees) {
    float theta = glm::radians(degrees);
    glm::vec4 u = m_u * glm::cos(theta) - m_w * glm::sin(theta);
    glm::vec4 w = m_u * glm::sin(theta) + m_w * glm::cos(theta);
    m_u = u;
    m_w = w;
    updateViewMatrix();
}

void CamtransCamera::rotateW(float degrees) {
    float theta = -glm::radians(degrees); // negative since rotating about -look
    glm::vec4 u = m_u * glm::cos(theta) - m_v * glm::sin(theta);
    glm::vec4 v = m_u * glm::sin(theta) + m_v * glm::cos(theta);
    m_u = u;
    m_v = v;
    updateViewMatrix();
}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    m_near = nearPlane;
    m_far = farPlane;
    updateProjectionMatrix();
}

void CamtransCamera::updateProjectionMatrix() {
    updateScaleMatrix();
    updatePerspectiveMatrix();
}

void CamtransCamera::updatePerspectiveMatrix() {
    float c = -(m_near / m_far);
    float first = -1.0f / (c + 1.0f);
    float second = c / (c + 1.0f);
    m_perspectiveTransformation = glm::transpose(glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                                                           0.0f, 1.0f, 0.0f, 0.0f,
                                                           0.0f, 0.0f, first, second,
                                                           0.0f, 0.0f, -1.0f, 0.0f));
}

void CamtransCamera::updateScaleMatrix() {
    float y = glm::cot(m_thetaH / 2.0f) / m_far;
    // when the m_aspectRatio changes, m_thetaW is not updated so we need this work-around
    float x = y * 1.0f / m_aspectRatio;
    float z = 1.0f / m_far;
    m_scaleMatrix = glm::transpose(glm::mat4(x, 0.0f, 0.0f, 0.0f,
                                             0.0f, y, 0.0f, 0.0f,
                                             0.0f, 0.0f, z, 0.0f,
                                             0.0f, 0.0f, 0.0f, 1.0f));
}

void CamtransCamera::updateViewMatrix() {
    updateTranslationMatrix();
    updateRotationMatrix();
}

void CamtransCamera::updateRotationMatrix() {
    m_rotationMatrix = glm::transpose(glm::mat4(m_u.x, m_u.y, m_u.z, 0.0f,
                                                m_v.x, m_v.y, m_v.z, 0.0f,
                                                m_w.x, m_w.y, m_w.z, 0.0f,
                                                0.0f, 0.0f, 0.0f, 1.0f));
}

void CamtransCamera::updateTranslationMatrix() {
    m_translationMatrix = glm::transpose(glm::mat4(1.0f, 0.0f, 0.0f, -m_eye.x,
                                                   0.0f, 1.0f, 0.0f, -m_eye.y,
                                                   0.0f, 0.0f, 1.0f, -m_eye.z,
                                                   0.0f, 0.0f, 0.0f, 1.0f));
}

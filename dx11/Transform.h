#pragma once
#include <DirectXMath.h>

class Transform {
public:
	Transform() = default;
	Transform(const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& position);
	~Transform() = default;

	Transform(const Transform&) = default;
	Transform& operator=(const Transform&) = default;
	
	Transform(Transform&&) = default;
	Transform& operator=(Transform&&) = default;

	// ��ȡ�������ű���
	DirectX::XMFLOAT3 GetScale() const;
	// ��ȡ�������ű���
	DirectX::XMVECTOR GetScaleXM() const;

	// ��ȡ����ŷ����(������)
	// ������Z-X-Y��˳����ת
	DirectX::XMFLOAT3 GetRotation() const;

	// ��ȡ����ŷ����(������)
	// ������Z-X-Y��˳����ת
	DirectX::XMVECTOR GetRotationXM() const;

    // ��ȡ����λ��
    DirectX::XMFLOAT3 GetPosition() const;
    // ��ȡ����λ��
    DirectX::XMVECTOR GetPositionXM() const;

    // ��ȡ�ҷ�����
    DirectX::XMFLOAT3 GetRightAxis() const;
    // ��ȡ�ҷ�����
    DirectX::XMVECTOR GetRightAxisXM() const;

    // ��ȡ�Ϸ�����
    DirectX::XMFLOAT3 GetUpAxis() const;
    // ��ȡ�Ϸ�����
    DirectX::XMVECTOR GetUpAxisXM() const;

    // ��ȡǰ������
    DirectX::XMFLOAT3 GetForwardAxis() const;
    // ��ȡǰ������
    DirectX::XMVECTOR GetForwardAxisXM() const;

    // ��ȡ����任����
    DirectX::XMFLOAT4X4 GetLocalToWorldMatrix() const;
    // ��ȡ����任����
    DirectX::XMMATRIX GetLocalToWorldMatrixXM() const;

    // ��ȡ������任����
    DirectX::XMFLOAT4X4 GetWorldToLocalMatrix() const;
    // ��ȡ������任����
    DirectX::XMMATRIX GetWorldToLocalMatrixXM() const;

    // ���ö������ű���
    void SetScale(const DirectX::XMFLOAT3& scale);
    // ���ö������ű���
    void SetScale(float x, float y, float z);

    // ���ö���ŷ����(������)
    // ������Z-X-Y��˳����ת
    void SetRotation(const DirectX::XMFLOAT3& eulerAnglesInRadian);
    // ���ö���ŷ����(������)
    // ������Z-X-Y��˳����ת
    void SetRotation(float x, float y, float z);

    // ���ö���λ��
    void SetPosition(const DirectX::XMFLOAT3& position);
    // ���ö���λ��
    void SetPosition(float x, float y, float z);

    // ָ��ŷ������ת����
    void Rotate(const DirectX::XMFLOAT3& eulerAnglesInRadian);
    // ָ����ԭ��Ϊ����������ת
    void RotateAxis(const DirectX::XMFLOAT3& axis, float radian);
    // ָ����pointΪ��ת����������ת
    void RotateAround(const DirectX::XMFLOAT3& point, const DirectX::XMFLOAT3& axis, float radian);

    // ����ĳһ����ƽ��
    void Translate(const DirectX::XMFLOAT3& direction, float magnitude);

    // �۲�ĳһ��
    void LookAt(const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });
    // ����ĳһ����۲�
    void LookTo(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });

private:
    // ����ת�����ȡ��תŷ����
    DirectX::XMFLOAT3 GetEulerAnglesFromRotationMatrix(const DirectX::XMFLOAT4X4& rotationMatrix);

private:
    DirectX::XMFLOAT3 m_Scale = { 1.0f, 1.0f, 1.0f };                // ����
    DirectX::XMFLOAT3 m_Rotation = {};                                // ��תŷ����(������)
    DirectX::XMFLOAT3 m_Position = {};                                // λ��
};
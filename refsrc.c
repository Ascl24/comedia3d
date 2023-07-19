/* void CameraYaw(Camera *camera, float angle, bool rotateAroundTarget)
{
    // Rotation axis
    Vector3 up = camera->up;

    // View vector
    Vector3 targetPosition = Vector3Subtract(camera->target, camera->position);

    // Rotate view vector around up axis
    targetPosition = Vector3RotateByAxisAngle(targetPosition, up, angle);

    if (rotateAroundTarget)
    {
        // Move position relative to target
        camera->position = Vector3Subtract(camera->target, targetPosition);
    }
    else // rotate around camera.position
    {
        // Move target relative to position
        camera->target = Vector3Add(camera->position, targetPosition);
    }
*/
/*
void UpdateCameraPro(Camera *camera, Vector3 movement, Vector3 rotation, float zoom)
{
    // Required values
    // movement.x - Move forward/backward
    // movement.y - Move right/left
    // movement.z - Move up/down
    // rotation.x - yaw
    // rotation.y - pitch
    // rotation.z - roll
    // zoom - Move towards target

    bool lockView = true;
    bool rotateAroundTarget = false;
    bool rotateUp = false;
    bool moveInWorldPlane = true;

    // Camera rotation
    CameraPitch(camera, -rotation.y*DEG2RAD, lockView, rotateAroundTarget, rotateUp);
    CameraYaw(camera, -rotation.x*DEG2RAD, rotateAroundTarget);
    CameraRoll(camera, rotation.z*DEG2RAD);

    // Camera movement
    CameraMoveForward(camera, movement.x, moveInWorldPlane);
    CameraMoveRight(camera, movement.y, moveInWorldPlane);
    CameraMoveUp(camera, movement.z);

    // Zoom target distance
    CameraMoveToTarget(camera, zoom);
}
*/
